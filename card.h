#ifndef _CARD_H_
#define _CARD_H_

#include <string>

class Card {
	private:
		std::string suit;
		std::string value;
		bool jokerAssigned; // true if its a joker that has been assigned a value
		bool inArray(int arraySize, std::string arr[], std::string s); // checks whether s is in arr (used to check that value and suit are valid)
	public:
		static std::string values[];
 		static std::string suits[];
		Card(std::string suit, std::string value);
		std::string getSuit();
		std::string getValue();
		std::string inspectCard();
		int intValue(); // returns the numerical value of the card
		void jokerUnassigned(); // call when head is discarded, sets jokerAssigned to false
		void setSuit(std::string suit); // may throw invalid_argument
		void setValue(std::string value); // may throw invalid_argument
};
#endif
