export module counter;

export import <cstdint>;
export import <queue>;
export using event_queue = std::priority_queue<std::pair<uint64_t, uint64_t>,
  						std::vector<std::pair<uint64_t,uint64_t>>,
  						std::greater<std::pair<uint64_t, uint64_t>>>;

export class counter {
 private:
  uint64_t div, value, ignore;	 // initially ignore & val = 0

 protected:
  uint64_t get_ignore() const noexcept {
    return ignore;
  }
  
  void set_ignore(uint64_t i) noexcept {
    ignore = i;
  }

  void set_value(uint64_t v) noexcept {
    value = v;
  }
	
  uint64_t get_div() const noexcept {
    return div;
  }
	
  uint64_t real_imp(uint64_t imp) noexcept {
    if (imp>ignore) {
      imp -= (ignore+1);	// imp>ignore, so ignore+1 <= UINT64_MAX
      if (div == UINT64_MAX) {
	ignore = div - imp;
	return 1;
      }
      ignore = div - imp%(div+1);
      return imp/(div+1) + 1;
    }
    else {
      ignore -= imp;
      return 0;
    }
  }

 public:
  counter() = delete;

  counter(uint64_t d) noexcept : div(d), value(0), ignore(0) {}

  uint64_t get_value() const noexcept {
    return value;
  }

  virtual void give_impulse(uint64_t imp, event_queue &q, uint64_t nr) = 0;

  virtual ~counter(){};
};

export class counterMod : public counter {
 private:
  uint64_t max_value;

 public:
  counterMod() = delete;

  counterMod(uint64_t d, uint64_t m) noexcept : counter(d), max_value(m) {}

  void give_impulse(uint64_t imp, event_queue &q, uint64_t nr) override {
    uint64_t imp_till_now = counter::get_ignore();
    uint64_t real_impulse = counter::real_imp(imp);
    uint64_t lost_imp = 0;

    // Overflow only when we don't use it later, so it's value is not important
    imp_till_now += (max_value - counter::get_value()) * (counter::get_div() + 1) + 1;

    if (UINT64_MAX - real_impulse < counter::get_value()) {
      lost_imp = real_impulse;
      real_impulse = UINT64_MAX - counter::get_value();
      lost_imp -= real_impulse;
    }

    counter::set_value(counter::get_value() + real_impulse);
		
    while (counter::get_value() > max_value) {
      q.push(std::make_pair(imp_till_now, nr));
      counter::set_value(counter::get_value() - max_value - 1);
      imp_till_now += counter::get_div();
      imp_till_now += max_value * (counter::get_div() + 1) + 1;
    }

    if (UINT64_MAX == counter::get_value() && lost_imp > 0) {
      q.push(std::make_pair(imp_till_now, nr));
      counter::set_value(0);
      lost_imp--;
    }

    counter::set_value(counter::get_value() + lost_imp);

    while (counter::get_value() > max_value) {
      q.push(std::make_pair(imp_till_now, nr));
      counter::set_value(counter::get_value() - max_value - 1);
      imp_till_now += counter::get_div();
      imp_till_now += max_value * (counter::get_div()+1) + 1;
    }
  }
};

export class counterFib : public counter {
 private:
  uint64_t fib1, fib2;
  bool stop;
	
 public:
  counterFib() = delete;

  counterFib(uint64_t d) noexcept : counter(d), fib1(1), fib2(1), stop(false) {}

  void give_impulse(uint64_t imp, event_queue &q, uint64_t nr) override {
    if (stop)
      return;

    uint64_t imp_till_now = counter::get_ignore();
    uint64_t real_impulse = counter::real_imp(imp);

    imp_till_now += (fib2 - counter::get_value() - 1) * (counter::get_div()+1) + 1;

    if (UINT64_MAX - real_impulse < counter::get_value()) {
      real_impulse = UINT64_MAX - counter::get_value();
    }

    counter::set_value(counter::get_value() + real_impulse);

    while (fib2 <= counter::get_value()) {
      q.push(std::make_pair(imp_till_now, nr));
      if (UINT64_MAX - fib2 < fib1) {
	stop = true;
	break;
      }

      uint64_t temp = fib2;
      fib2 += fib1;
      fib1 = temp;
      imp_till_now += counter::get_div();
      imp_till_now += (fib2 - fib1 - 1)*(counter::get_div() + 1) + 1;
    }
  }

};

export class counterGeoDec : public counter {
 private:
  uint64_t current_cycle;
 public:
  counterGeoDec() = delete;

  counterGeoDec(uint64_t d) noexcept : counter(d), current_cycle(9) {}

  void give_impulse(uint64_t imp, event_queue &q, uint64_t nr) override {
    uint64_t imp_till_now = counter::get_ignore();
    uint64_t real_impulse = counter::real_imp(imp);
    uint64_t lost_imp = 0;
    imp_till_now += (current_cycle - counter::get_value())*(counter::get_div() + 1) + 1;
	  
    if (UINT64_MAX - real_impulse < counter::get_value()) {
      lost_imp = real_impulse;
      real_impulse = UINT64_MAX - counter::get_value();
      lost_imp -= real_impulse;
    }
	  
    counter::set_value(counter::get_value() + real_impulse);

    while (counter::get_value() > current_cycle) {    
      q.push(std::make_pair(imp_till_now, nr));
      counter::set_value(counter::get_value() - current_cycle - 1);
	      
      if (current_cycle == 999999999999) {
	current_cycle = 9;
      }
      else {
	current_cycle *= 10;
	current_cycle += 9;
      }

      imp_till_now += counter::get_div();
      imp_till_now += current_cycle * (counter::get_div() + 1) + 1;
    }

    counter::set_value(counter::get_value() + lost_imp);
    while (counter::get_value() > current_cycle) {   
      q.push(std::make_pair(imp_till_now, nr));
      counter::set_value(counter::get_value() - current_cycle - 1);
	      
      if (current_cycle == 999999999999) {
	current_cycle = 9;
      }
      else {
	current_cycle *= 10;
	current_cycle += 9;
      }
	    
      imp_till_now += counter::get_div();
      imp_till_now += current_cycle*(counter::get_div() + 1) + 1;
    }
  }
};
