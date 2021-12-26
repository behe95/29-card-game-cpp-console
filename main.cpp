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
        Player* dealer = NULL;

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

    void connect_player(Player** dealer,string player_name)
    {
        //at the first time of the game
        //there is no player
        //so we are going to set the first created player as the dealer
        if(*dealer == NULL)
        {
            Player* player = new Player(player_name);
            (*player).right_hand_player = player;
            (*player).left_hand_player = player;
            *dealer = player;
            return;
        }


        //if dealer exists
        //then the following line of the codes will be executed

        //get the last added player
        //which is sitting right next to the dealer
        //or at the right hand side of the dealer
        Player* last_player = (*dealer)->right_hand_player;

        //create the new player
        Player* player = new Player(player_name);

        //this new player is going to be the last player now
        //this player's left hand side player will be the dealer
        (*player).left_hand_player = *dealer;

        //and the dealers right hand side player will be now the newly
        //created player
        (*dealer)->right_hand_player = player;

        //now newly players right hand side player will be
        //the previous last player who was sitting right next to the dealer
        (*player).right_hand_player = last_player;

        //and the previous last players left hand side player
        //will be now newly created player
        last_player->left_hand_player = player;

    }

    void create_players()
    {
        string player_name;
        cout << "Select player name:\t";
        cin >> player_name;

        connect_player(&dealer, player_name);
        connect_player(&dealer, "Player 2");
        connect_player(&dealer, "Player 3 (Team)");
        connect_player(&dealer, "Player 4");
    }



};


void draw_diamonds();
void draw_hearts();






int main()
{




    Board board = Board();

    board.create_players();



    //board.shuffle_cards();
    //board.get_cards();

    return 0;
}





