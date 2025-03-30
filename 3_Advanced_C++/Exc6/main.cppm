export module main;
import counter_collection;
import io;
import counter;
import <iostream>;

int main() {
  counterCollection collection;
  std::string line;
  u_int64_t line_nr = 0;
  while (getline(std::cin, line)) {
    line_nr++;
    parse_line(line, line_nr, collection);
  }

  return 0;
}
