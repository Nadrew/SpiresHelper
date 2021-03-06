#include "SpiresHelper.h"

namespace SpiresHelper {
	unordered_map<std::string, ofstream*> file_handlers;
	int files_open = 0;

	void CheckStack() {
		if (files_open > MAX_HANDLERS) {
			ofstream *handler = file_handlers.begin()->second;
			handler->close();
			files_open--;
			file_handlers.erase(file_handlers.begin());
		}
	}

	inline boost::tuple<boost::gregorian::date, boost::posix_time::time_duration> timestamp() {
		boost::posix_time::ptime timeLocal = boost::posix_time::second_clock::local_time();
		boost::gregorian::date dateObj = timeLocal.date();
		boost::posix_time::time_duration timeObj = timeLocal.time_of_day();
		return boost::make_tuple(dateObj, timeObj);
	}

	void *LogText(int argc, char *args[]) {
		char *l_path = args[0];
		char *log_string = args[1];
		if (l_path == NULL || strlen(l_path) == 0) {
			return false;
		}
		char *is_path = strchr(l_path, '/');
		std::string log_path = l_path;
		if (is_path != NULL) {
			boost::filesystem::path log_path_no_file(log_path);
			boost::filesystem::path dir = log_path_no_file.parent_path();
			boost::filesystem::create_directories(dir);
		}
		ofstream *handler;
		std::unordered_map<std::string, ofstream*>::iterator it;
		it = file_handlers.find(log_path);
		if (it != file_handlers.end()) {
			handler = it->second;
		}
		else {
			handler = new ofstream(l_path, ios::app);
			file_handlers[log_path] = handler;
		}
		
		if (handler) {
			if (!handler->is_open()) {
				handler->open(l_path);
				files_open++;
			}
			boost::tuple<boost::gregorian::date, boost::posix_time::time_duration> ts = timestamp();
			boost::gregorian::date date = ts.get<0>();
			boost::posix_time::time_duration time = ts.get<1>();
			*handler << '[' << date << " " << time << ']' << '\n' << log_string << endl;
			
		}
		CheckStack();
		return false;
	}

	void *Unregister(int argc, char *args[]) {
		char *path = args[0];
		std::unordered_map<std::string, ofstream*>::iterator it;

		it = file_handlers.find(path);
		if (it != file_handlers.end()) {
			ofstream *handler = it->second;
			handler->close();
			files_open--;
			file_handlers.erase(it);
		}
		return false;
	}

	void *UnregisterAll(int argc, char *args[]) {
		for (unordered_map<std::string, ofstream*>::iterator it = file_handlers.begin(); it != file_handlers.end(); ++it) {
			ofstream *handler = it->second;
			handler->close();
		}
		files_open = 0;
		file_handlers.clear();
		return false;
	}
}