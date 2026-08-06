#include <cmath>
#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;

int Case;
bool Flag[4][19];

struct Card
{
	int suit, rank;
	Card() : Card(0, 0) {}
	Card(char suit_ch, char rank_ch) : Card(GetSuit(suit_ch), GetRank(rank_ch)) {}
	Card(int suit, int rank) : suit(suit), rank(rank) {}
	static int GetSuit(char ch)
	{
		if (ch == 'C')
			return 0;
		if (ch == 'D')
			return 1;
		if (ch == 'H')
			return 2;
		return 3;
	}
	static int GetRank(char ch)
	{
		if (ch == 'A')
			return 14;
		if (ch == 'K')
			return 13;
		if (ch == 'Q')
			return 12;
		if (ch == 'J')
			return 11;
		if (ch == 'T')
			return 10;
		return ch - '0';
	}
	bool operator < (const Card &rhs) const
	{
		return rank < rhs.rank;
	}
	bool operator == (const Card &rhs) const
	{
		return rank == rhs.rank && suit == rhs.suit;
	}
}A[9];

struct CombEval
{
	enum class Type : int
	{
		Unspecified = 0,
		HighCard,
		Pair,
		TwoPairs,
		Three,
		Straight,
		Flush,
		FullHouse,
		Four,
		StraightFlush,
		RoyalFlush,
	};
	CombEval() : CombEval(Type::Unspecified, {}) {}
	CombEval(Type type, const vector<Card> &cards) : type(type), cards(cards) {}
	
	bool TryHighCard()
	{
		return true;
	}
	bool TryPair()
	{
		int id = -1;
		for (int i = 0; id == -1 && i <= 3; i ++)
			if (cards[i].rank == cards[i + 1].rank)
				id = i;
		if (id == -1)
			return false;
		int main_rank = cards[id].rank;
		for (int i = 0, cnt = 0; i <= 4; i ++)
			if (cards[i].rank != main_rank)
			{
				for (int j = i; j > cnt; j --)
					swap(cards[j], cards[j - 1]);
				cnt ++;
			}
		return true;
	}
	bool TryTwoPairs()
	{
		int id1 = -1, id2 = -1;
		for (int i = 0; i <= 3; i ++)
			if (cards[i].rank == cards[i + 1].rank)
			{
				if (id1 == -1)
					id1 = i;
				else id2 = i;
				i ++;
			}
		if (id1 == -1 || id2 == -1)
			return false;
		for (int j = 8 - id1 * 2 - id2 * 2; j; j --)
			swap(cards[j], cards[j - 1]);
		return true;
	}
	bool TryThree()
	{
		int id = -1;
		for (int i = 0; id == -1 && i <= 2; i ++)
			if (cards[i].rank == cards[i + 1].rank && cards[i].rank == cards[i + 2].rank)
				id = i;
		if (id == -1)
			return false;
		int main_rank = cards[id].rank;
		for (int i = 0, cnt = 0; i <= 4; i ++)
			if (cards[i].rank != main_rank)
			{
				for (int j = i; j > cnt; j --)
					swap(cards[j], cards[j - 1]);
				cnt ++;
			}
		return true;
	}
	bool TryStraight()
	{
		for (int i = 1; i <= 3; i ++)
			if (cards[i].rank != cards[i - 1].rank + 1)
				return false;
		if (cards[4].rank == cards[3].rank + 1)
			return true;
		if (cards[4].rank == 14 && cards[0].rank == 2)
		{
			for (int i = 4; i; i --)
				swap(cards[i], cards[i - 1]);
			return true;
		}
		return false;
	}
	bool TryFlush()
	{
		for (const Card &card : cards)
			if (card.suit != cards[0].suit)
				return false;
		return true;
	}
	bool TryFullHouse()
	{
		if (!TryThree())
			return false;
		return cards[0].rank == cards[1].rank;
	}
	bool TryFour()
	{
		if (!TryThree())
			return false;
		return cards[1].rank == cards[2].rank;
	}
	bool TryStraightFlush()
	{
		return TryFlush() && TryStraight();
	}
	bool TryRoyalFlush()
	{
		if (!TryStraightFlush())
			return false;
		if (cards[0].rank != 10)
			return false;
		return true;
	}
	
	void Sort()
	{
		sort(cards.begin(), cards.end());
	}
	
	bool operator < (const CombEval &rhs) const
	{
		if (type != rhs.type)
			return int(type) < int(rhs.type);
		for (int i = 4; i >= 0; i --)
			if ((cards[i] < rhs.cards[i]) || (rhs.cards[i] < cards[i]))
				return cards[i] < rhs.cards[i];
		return false;
	}
	bool operator == (const CombEval &rhs) const
	{
		return !(*this < rhs) && !(rhs < *this);
	}
	Type type;
	vector<Card> cards;
};

CombEval GetCombEval(const vector<Card> &cards)
{
	CombEval ret;
	ret.cards = cards;
	ret.Sort();
	bool ok = false;
	if (!ok && ret.TryRoyalFlush()) ret.type = CombEval::Type::RoyalFlush, ok = true;
	ret.Sort();
	if (!ok && ret.TryStraightFlush()) ret.type = CombEval::Type::StraightFlush, ok = true;
	if (!ok && ret.TryFour()) ret.type = CombEval::Type::Four, ok = true;
	if (!ok && ret.TryFullHouse()) ret.type = CombEval::Type::FullHouse, ok = true;
	if (!ok && ret.TryFlush()) ret.type = CombEval::Type::Flush, ok = true;
	if (!ok && ret.TryStraight()) ret.type = CombEval::Type::Straight, ok = true;
	if (!ok && ret.TryThree()) ret.type = CombEval::Type::Three, ok = true;
	if (!ok && ret.TryTwoPairs()) ret.type = CombEval::Type::TwoPairs, ok = true;
	if (!ok && ret.TryPair()) ret.type = CombEval::Type::Pair, ok = true;
	if (!ok && ret.TryHighCard()) ret.type = CombEval::Type::HighCard, ok = true;
	return ret;
}

void CustomTest()
{
	vector<vector<Card>> test_cases = {
		{Card(0, 2), Card(0, 3), Card(0, 4), Card(0, 5), Card(1, 12)},	// High
		{Card(0, 2), Card(0, 3), Card(0, 4), Card(0, 5), Card(1, 3)},	// Pair
		{Card(1, 2), Card(3, 3), Card(0, 4), Card(1, 2), Card(2, 4)},	// TwoPairs
		{Card(0, 2), Card(0, 3), Card(0, 4), Card(2, 3), Card(1, 3)},	// Three
		{Card(0, 2), Card(0, 3), Card(0, 4), Card(0, 5), Card(1, 14)},	// Straight
		{Card(0, 2), Card(0, 3), Card(0, 4), Card(0, 6), Card(1, 5)},	// Straight
		{Card(0, 2), Card(0, 3), Card(0, 4), Card(0, 5), Card(0, 12)},	// Flush
		{Card(2, 3), Card(0, 3), Card(1, 5), Card(0, 5), Card(1, 3)},	// FullHouse
		{Card(2, 3), Card(0, 3), Card(1, 5), Card(3, 3), Card(1, 3)},	// Four
		{Card(0, 2), Card(0, 3), Card(0, 4), Card(0, 5), Card(0, 14)},	// StraightFlush
		{Card(0, 7), Card(0, 9), Card(0, 8), Card(0, 6), Card(0, 10)},	// StraightFlush
		{Card(0, 14), Card(0, 12), Card(0, 11), Card(0, 13), Card(0, 10)},	// RoyalFlush
	};
	for (const vector<Card> &cards : test_cases)
	{
		CombEval eval = GetCombEval(cards);
		fprintf(stderr, "type : %d, Cards :", int(eval.type));
		for (const Card &card : eval.cards)
			fprintf(stderr, " (%d, %d)", card.suit, card.rank);
		fprintf(stderr, "\n");
	}
	exit(0);
}

int main()
{
	//CustomTest();
	for (scanf("%d", &Case); Case; Case --)
	{
		for (int s = 0; s < 4; s ++)
			for (int r = 0; r <= 14; r ++)
				Flag[s][r] = false;
		vector<Card> cs, ps;
		for (int i = 1; i <= 8; i ++)
		{
			static char str[9];
			scanf("%s", str);
			A[i] = Card(str[1], str[0]);
			Flag[A[i].suit][A[i].rank] = true;
			if (i <= 4)
				cs.push_back(A[i]);
			else ps.push_back(A[i]);
		}
		Card c_1, c_2, p_1;
		CombEval ec_1, ec_2, ep_1, ep_2;
		for (int s = 0; s < 4; s ++)
			for (int r = 2; r <= 14; r ++)
				if (!Flag[s][r])
				{
					Card c = Card(s, r);
					cs.push_back(c);
					ps.push_back(c);
					CombEval ec = GetCombEval(cs), ep = GetCombEval(ps);
					if (c_2 == Card() || ec_2 < ec)
						c_2 = c, ec_2 = ec;
					if (c_1 == Card() || ec_1 < ec_2)
						swap(c_1, c_2), swap(ec_1, ec_2);
					if (p_1 == Card() || ep_1 < ep)
						p_1 = c, ep_1 = ep;
					cs.pop_back();
					ps.pop_back();
				}
		ps.push_back(c_1);
		ep_2 = GetCombEval(ps);
		int verdict = 0;
		if ((c_1 == p_1 ? ec_2 : ec_1) < ep_1 || ec_2 < ep_2)
			verdict = 1;
		if (ep_1 < (c_1 == p_1 ? ec_2 : ec_1) && ep_2 < ec_2)
			verdict = -1;
		puts(verdict == -1 ? "WoYaoYanPai" : (verdict == 1 ? "GeiWoCaPiXie" : "PaiMeiYouWenTi"));
	}
	return 0;
}

