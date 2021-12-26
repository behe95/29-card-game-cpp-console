#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <ctime>
#include <cstdlib>

using namespace std;


enum SUITS
{
    DIAMONDS_RED = 0,
    CLUBS_BLACK,
    HEARTS_RED,
    SPADES_BLACK
};

enum PLAYING_CARDS_RANKS
{
    JACK = 0,
    NINE,
    ACE,
    TEN,
    KING,
    QUEEN,
    EIGHT,
    SEVEN
};

enum CARD_POINTS
{
    ZERO = 0,
    ONE,
    TWO,
    THREE
};

vector<string> suits = {"D", "C", "H", "S"};
vector<string> playing_cards = {"J", "9", "A", "10", "K", "Q", "8", "7"};

class Card
{
    public:
    string card_name;
    string card_suit;
    int card_rank;
    int card_points;

    Card(string card_name, string card_suit, int card_rank, int card_points)
    {
        this->card_name = card_name;
        this->card_suit = card_suit;
        this->card_rank = card_rank;
        this->card_points = card_points;
    }
};


class Player
{
    private:
        string name;
        int score = 0;
        int bid = 0;

    public:
        Player* right_hand_player;
        Player* left_hand_player;

        Player(string name)
        {
            this->name = name;
        }

        string get_player_name()
        {
            return this->name;
        }

        int get_player_score()
        {
            return this->score;
        }

        void set_player_score(int score)
        {
            this->score = score;
        }

        int get_bid()
        {
            return this->bid;
        }

        void set_bid(int bid)
        {
            this->bid = bid;
        }
};

class Board
{
    private:
        int total_cards = 32;
        vector<Card> cards;
        Player* dealer;

    public:


    Board()
    {
        int card_point = 0;
        for(int suit = 0; suit < suits.size(); suit++)
        {
            for(int playing_card = 0; playing_card < playing_cards.size(); playing_card++)
            {
                switch(playing_card)
                {
                    case PLAYING_CARDS_RANKS::JACK:
                        card_point = 3;
                        break;
                    case PLAYING_CARDS_RANKS::NINE:
                        card_point = 2;
                        break;
                    case PLAYING_CARDS_RANKS::ACE:
                        card_point = 1;
                        break;
                    case PLAYING_CARDS_RANKS::TEN:
                        card_point = 1;
                        break;
                    default:
                        card_point = 0;
                }

                Card card = Card(playing_cards[playing_card], suits[suit], playing_card, card_point);
                cards.push_back(card);
            }
        }
    }

    void shuffle_cards()
    {
        srand(time(0));

        vector<Card> shuffled_cards;

        while(cards.size() != 0)
        {
            int random_selected = rand() % cards.size();
            shuffled_cards.push_back(cards[random_selected]);
            cards.erase(cards.begin() + random_selected);
        }

        for(auto card : shuffled_cards)
            cards.push_back(card);
    }

    void get_cards()
    {
        for(int i = 0; i < cards.size(); i++)
        {
            cout << cards[i].card_name << "-" << cards[i].card_suit << "\t";
        }
    }

    void create_players()
    {
        string player_name;
        cout << "Select player name:\t";
        cin >> player_name;

    }

};


void draw_diamonds();
void draw_hearts();






int main()
{





    //Player player1 = Player(player_name);
    //Player player2 = Player("Player 2");
    //Player player3 = Player("Player 3 (Team Mate)");
    //Player player4 = Player("Player 4");

    //Player players[] = {player1, player2, player3, player4};
    //int total_players = sizeof(players) / sizeof(*players);

    Board board = Board();

    board.create_players();

    board.shuffle_cards();
    board.get_cards();

    return 0;
}

void draw_hearts()
{
    float total_rows = 6;
    float total_columns = 7;

    for(int i = 0; i < ceil(total_rows/2); i++)
    {

    }
}

void draw_diamonds()
{

    float total_rows = 7;
    int half_space_columns;

    for(int i = 0; i < ceil(total_rows/2) - 1; i++)
    {
        half_space_columns = abs(ceil(total_rows/2) - i);
        cout << string(half_space_columns, ' ');
        cout << string (1 + 2*i, '*');
        cout << string(half_space_columns, ' ');
        cout << endl;
    }
    for(int i = ceil(total_rows/2) - 1; i >= 0; i--)
    {
        half_space_columns = abs(ceil(total_rows/2) - i);
        cout << string(half_space_columns, ' ');
        cout << string (1 + 2*i, '*');
        cout << string(half_space_columns, ' ');
        cout << endl;
    }
}


