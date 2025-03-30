export module io;
import counter_collection;
import counter;
import <iostream>;
import <regex>;

void error(u_int64_t line_nr) {
  std::cerr << "ERROR " << line_nr << std::endl;
}

export void parse_line(std::string const &line, u_int64_t line_nr, counterCollection &collection) {
  if (line.empty()) error(line_nr);

  switch (line[0]) {
  case 'M': {
    std::regex r("M ([0-9]+) ([0-9]+) ([0-9]+)");
    std::smatch matches;
    if (!std::regex_match(line, matches, r)) {
      error(line_nr);
    }
    else {
      try {
	counterMod c(std::stoull(matches[2]), std::stoull(matches[3]));
	if (!collection.insert(std::stoul(matches[1]), std::make_unique<counterMod>(c))){
	  error(line_nr);
	}
      }
      catch (const std::exception) {
	error(line_nr);
      }

    }
    break;
  }

  case 'F': {
    std::regex r("F ([0-9]+) ([0-9]+)");
    std::smatch matches;
    if (!std::regex_match(line, matches, r)) {
      error(line_nr);
    }
    else {
      try {
	counterFib c(std::stoull(matches[2]));
	if (!collection.insert(std::stoul(matches[1]), std::make_unique<counterFib>(c))) {
	  error(line_nr);
	}
      }
      catch (const std::exception) {
	error(line_nr);
      }
    }
    break;
  }
    
  case 'G': {
    std::regex r("G ([0-9]+) ([0-9]+)");
    std::smatch matches;
    if (!std::regex_match(line, matches, r)) {
      error(line_nr);
    }
    else {
      try {
	counterGeoDec c(std::stoull(matches[2]));
	if (!collection.insert(std::stoul(matches[1]), std::make_unique<counterGeoDec>(c))) {
	  error(line_nr);
	}
      }
      catch (const std::exception) {
	error(line_nr);
      }

    }
    break;
  }
    
  case 'D': {
    std::regex r("D ([0-9]+)");
    std::smatch matches;
    if (!std::regex_match(line, matches, r)) {
      error(line_nr);
    }
    else {
      try {
	if (!collection.erase(std::stoull(matches[1]))) {
	  error(line_nr);
	}
      }
      catch (const std::exception) {
	error(line_nr);
      }
    }
    break;
  }
    
  case 'P': {
    std::regex r("P ([0-9]+)");
    std::smatch matches;
    if (!std::regex_match(line, matches, r)) {
      error(line_nr);
      return;
    }
    else {
      try {
	if (!collection.print_counter(std::stoull(matches[1]))) {
	  error(line_nr);
	}
      }
      catch (const std::exception) {
	error(line_nr);
      }
    }
    break;
  }
    
  case 'A': {
    std::regex r("A ([0-9]+)");
    std::smatch matches;
    if (!std::regex_match(line, matches, r)) {
      error(line_nr);
    }
    else {
      try {
	uint64_t imp = std::stoull(matches[1]);
	if (imp == 0) {
	  return;
	}
	collection.give_impulse(imp);
      }
      catch (const std::exception) {
	error(line_nr);
      }
    }
    break;
  }
  default:
    error(line_nr);
  }
}
