#include "SpiresHelper.h"

namespace SpiresHelper {
	map<char*, ofstream*> file_handlers;

	void CheckStack() {
		if (file_handlers.size() > MAX_HANDLERS) {
			file_handlers[0]->close();
			file_handlers.erase(0);
		}
	}

	inline boost::tuple<boost::gregorian::date, boost::posix_time::time_duration> timestamp() {
		boost::posix_time::ptime timeLocal = boost::posix_time::second_clock::local_time();
		boost::gregorian::date dateObj = timeLocal.date();
		boost::posix_time::time_duration timeObj = timeLocal.time_of_day();
		return boost::make_tuple(dateObj, timeObj);
	}

	void *LogText(int argc, char *args[]) {
		char *log_path = args[0];
		char *log_string = args[1];
		char *is_path = strchr(log_path, '/');
		if (is_path != NULL) {
			boost::filesystem::path log_path_no_file(log_path);
			boost::filesystem::path dir = log_path_no_file.parent_path();
			boost::filesystem::create_directories(dir);
		}
		ofstream *handler;
		if (file_handlers.find(log_path) != file_handlers.end()) {
			handler = file_handlers[log_path];
		}
		else {
			handler = new ofstream(log_path, ios::app);
			file_handlers[log_path] = handler;
		}
		CheckStack();
		if (handler) {
			if (!handler->is_open()) handler->open(log_path);
			boost::tuple<boost::gregorian::date, boost::posix_time::time_duration> ts = timestamp();
			boost::gregorian::date date = ts.get<0>();
			boost::posix_time::time_duration time = ts.get<1>();
			*handler << '[' << date << " " << time << ']' << '\n' << log_string << endl;
		}
		return false;
	}

	void *Unregister(int argc, char *args[]) {
		char *path = args[0];
		if (file_handlers.find(path) != file_handlers.end()) {
			file_handlers[path]->close();
			file_handlers.erase(path);
		}
		return false;
	}

	void *UnregisterAll(int argc, char *args[]) {
		for (map<char*, ofstream*>::iterator it = file_handlers.begin(); it != file_handlers.end(); ++it) {
			ofstream *handler = it->second;
			handler->close();
		}
		file_handlers.clear();
		return false;
	}
}