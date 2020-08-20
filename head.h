#ifndef _HEAD_H_
#define _HEAD_H_
#include "deck.h"

class Head: public Deck {
	private:
		int headNum;
		static int instances;
	public:
		Head();
		int getHeadNum();
		int getTopIntValue();
		std::string topCard();

};

#endif
