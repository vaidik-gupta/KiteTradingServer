#include <BufferManager.h>

BufferManager::~BufferManager() {
	std::lock_guard<std::mutex> lock(list_mutex);
	for (auto& ts : time_series_list) {
		ts.running = false;
		if (ts.worker.joinable()) {
			ts.worker.join();
		}
	}
	time_series_list.clear();
}

void BufferManager::startTimeSeries(std::list<TimeSeries>::iterator it) {

    it->running = true;
    it->worker = std::thread([it]() {
        while (it->running) {
            if (it->source->isRunning()) {
                double value = it->source->getValue();
                it->ts->add_data(value);
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(it->interval));
        }
    });
}

std::list<TimeSeries>::iterator BufferManager::addTimeSeries(
	std::unique_ptr<TimeSeriesBase> ts,
	std::shared_ptr<TickSource> source,
	uint64_t interval)
{
	std::lock_guard<std::mutex> lock(list_mutex);
	time_series_list.emplace_back(TimeSeries{std::move(ts), source, interval, std::thread(), false});
	auto it = std::prev(time_series_list.end());
    startTimeSeries(it);
	return it;
}

void BufferManager::removeTimeSeries(std::list<TimeSeries>::iterator it) {
	std::lock_guard<std::mutex> lock(list_mutex);
	it->running = false;
	if (it->worker.joinable()) {
		it->worker.join();
	}
	time_series_list.erase(it);
}
