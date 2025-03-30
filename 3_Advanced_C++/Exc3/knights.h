#ifndef KNIGHTS_H
#define KNIGHTS_H

#include <ostream>
#include <utility>
#include <deque>
#include <algorithm>
#include <limits>
#include <cmath>
#include <functional>
#include <compare>

class Knight {
    private:

	    size_t gold, weapon, armour;
	
	public:

        constexpr static size_t MAX_GOLD = std::numeric_limits<size_t>::max();

		constexpr Knight(size_t g, size_t w, size_t a) : gold(g), weapon(w), armour(a) {}

        constexpr Knight(Knight const& k) = default;
        constexpr Knight(Knight&& k) = default;

		constexpr size_t get_gold(void) const {
            return gold;
        }
		constexpr size_t get_weapon_class(void) const {
            return weapon;
        }
		constexpr size_t get_armour_class(void) const {
            return armour;
        }
		
		constexpr void take_gold(size_t to_take) {
			if (MAX_GOLD - gold <= to_take)
				gold = MAX_GOLD;
			else
				gold += to_take;
		}
	
		constexpr size_t give_gold(void) {
            size_t res = gold;
            gold = 0;
            return res;
        }

		constexpr void change_weapon(size_t new_weapon) {
            weapon = new_weapon;
        }
		constexpr size_t give_up_weapon(void) {
            size_t res = weapon;
            weapon = 0;
            return res;
        }
		constexpr void change_armour(size_t new_armour) {
            armour = new_armour;
        }
		constexpr size_t take_off_armour(void) {
            size_t res = armour;
            armour = 0;
            return res;
        }
		
        constexpr Knight& operator=(Knight const& k) & = default;
        constexpr Knight& operator=(Knight const& k) && = delete;
        constexpr Knight& operator=(Knight&& k) & = default;
        constexpr Knight& operator=(Knight&& k) && = delete;

		constexpr Knight& operator+=(Knight& k) & {
			take_gold(k.give_gold());
			
			if (weapon < k.weapon)
				change_weapon(k.give_up_weapon());

			if (armour < k.armour)
				change_armour(k.take_off_armour());
			
			return *this;
		}
        constexpr Knight& operator+=(Knight& k) && = delete;

        constexpr Knight& operator+=(Knight&& k) & {
            take_gold(k.give_gold());

            if (weapon < k.weapon)
                change_weapon(k.give_up_weapon());

            if (armour < k.armour)
                change_armour(k.take_off_armour());

            return *this;
        }
        constexpr Knight& operator+=(Knight&& k) && = delete;
	
		constexpr const Knight operator+(Knight const& k) const {
			Knight res = *this;
			Knight k_copy = k;
			res += k_copy;
			
			return res;
		}

        

        // Compare knights' combat parameters
        constexpr auto operator<=>(Knight const& k) const {
            auto ret = std::weak_ordering::equivalent;
            
            if (weapon > k.armour && armour >= k.weapon)
                ret = std::weak_ordering::greater;
            else if (weapon <= k.armour && armour < k.weapon)
                ret = std::weak_ordering::less;
            else if (weapon <= k.armour && k.weapon <= armour) {}
            else if (armour != k.armour)
                ret = armour <=> k.armour;
            else if (weapon != k.weapon)
                ret = weapon <=> k.weapon;
            
            // If we have not entered any of the "if"/"else if" we return strong_ordering::equal
            
            return ret;
        }

        // Check if the knights' battle would result in a draw
        constexpr bool operator==(Knight const& k) const {
            return *this <=> k == std::weak_ordering::equivalent;
        }
}; // Knight

inline std::ostream& operator<<(std::ostream& os, Knight const& k) {
    os << '(' << k.get_gold() << " gold, " 
        << k.get_weapon_class() << " weapon class, " 
        << k.get_armour_class() << " armour class)" << std::endl;
        
    return os;
}

inline constexpr Knight TRAINEE_KNIGHT{0, 1, 1};

class Tournament {
    private:

        // Tournament lists are implemented with std::deque instead of std::list
        // for better performance, since there are multiple end insertions and pops
        // and no in-the-middle insertions
        std::deque<Knight> pretenders;
        std::deque<Knight> losers;

	public:

        Tournament(std::initializer_list<Knight> const& knight_list) : pretenders{knight_list},
                                                                        losers{} {
            if (pretenders.empty())
                pretenders.push_back(TRAINEE_KNIGHT);
        }

        Tournament(Tournament const& t) = default;
        Tournament(Tournament&& t) = default;

        size_t size(void) const {
            return pretenders.size() + losers.size();
        }

        std::deque<Knight>::const_iterator no_winner(void) const & {
            return pretenders.end();
        }
        std::deque<Knight>::const_iterator no_winner(void) const && = delete;

        Tournament& operator=(Tournament const& t) & = default;
        Tournament& operator=(Tournament const& t) && = delete;
        Tournament& operator=(Tournament&& t) & = default;
        Tournament& operator=(Tournament&& t) && = delete;

        Tournament& operator+=(Knight const& k) & {
            pretenders.push_back(k);
            losers.clear();
            return *this;
        }
        Tournament& operator+=(Knight const& k) && = delete;

        // Erase all knights that have *all* parameters equal to k's
        Tournament& operator-=(Knight const& k) & {
            std::erase_if(pretenders, [&k](Knight const& x) {
                        return k.get_gold() == x.get_gold()
                        && k.get_weapon_class() == x.get_weapon_class()
                        && k.get_armour_class() == x.get_armour_class();});
            losers.clear();
            return *this;
        }
        Tournament& operator-=(Knight const& k) && = delete;

        std::deque<Knight>::const_iterator play(void) & {
            losers.clear();
            while (pretenders.size() > 1) {
                Knight& k1 = pretenders.front();
                pretenders.pop_front();
                Knight& k2 = pretenders.front();
                pretenders.pop_front();
                if (k1 > k2) {
                    k1 += k2;
                    pretenders.push_back(k1);
                    losers.push_front(k2);
                }
                else if (k1 < k2) {
                    k2 += k1;
                    pretenders.push_back(k2);
                    losers.push_front(k1);
                }
                else {
                    losers.push_front(k2);
                    losers.push_front(k1);
                }
            }

            return pretenders.size() == 1 ? pretenders.begin() : no_winner();
        }
        std::deque<Knight>::const_iterator play(void) && = delete;

        friend std::ostream& operator<< (std::ostream& os, Tournament const& t) {
            for (auto it = t.pretenders.begin(); it != t.pretenders.end(); ++it)
                os << "+ " << *it;

            for (auto it = t.losers.begin(); it != t.losers.end(); ++it)
                os << "- " << *it;

            os << '=' << std::endl;
            
            return os;
        }
        
}; // Tournament

namespace {   
consteval size_t class_diff(Knight const& k) {
    size_t k_armour = k.get_armour_class(), k_weapon = k.get_weapon_class();
    return k_armour >= k_weapon ? k_armour - k_weapon : k_weapon - k_armour;
}

consteval bool classes_cmp(Knight const& k1, Knight const& k2) {
    return std::less<size_t>{}(class_diff(k1), class_diff(k2));
}
} // namespace

consteval std::pair<size_t, size_t> max_diff_classes(std::initializer_list<Knight> knight_list) {
    Knight const& res = std::max(knight_list, classes_cmp);
    return {res.get_weapon_class(), res.get_armour_class()};
}


#endif
