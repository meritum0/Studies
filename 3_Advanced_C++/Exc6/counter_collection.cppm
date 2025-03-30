export module counter_collection;

export import <memory>;
export import <unordered_map>;
import <iostream>;
import counter;

export class counterCollection {
 private:
  std::unordered_map<uint64_t, std::unique_ptr<counter>> counters;
  event_queue events;	// pair of <time> and <nr> in that order

 public:
  counterCollection() = default;

  bool insert(uint64_t nr, std::unique_ptr<counter> c) {
    auto it = counters.find(nr);
    if (it != counters.end()) {
      return false;
    }

    counters[nr] = std::move(c);
    return true;
  }

  bool erase(uint64_t nr) {
    auto it = counters.find(nr);

    if (it == counters.end()) {
      return false;
    }

    counters.erase(it);
    return true;
  }

  bool print_counter(uint64_t nr) {
    auto it = counters.find(nr);
    uint64_t val;
    if (it == counters.end()) {
      return false;
    }
    else {
      val = it->second->get_value();
    }

    std::cout << "C" << " " << nr << " " << val << "\n";
    return true;
  }

  void give_impulse(uint64_t imp) {
    for (auto it = counters.begin(); it != counters.end(); it++) {
      it->second->give_impulse(imp, events, it->first);
    }

    while (!events.empty()) {
      std::cout << "E" << " "
		<< events.top().second << " "
		<< events.top().first << "\n";

      events.pop();
    }
  }
};
