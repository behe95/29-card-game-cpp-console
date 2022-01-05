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
        bool player_passed = false;
        vector<Card> cards_in_hand;

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

        void set_cards_in_hand(Card card)
        {
            this->cards_in_hand.push_back(card);
        }

        void show_cards_in_hand()
        {
            cout << "====================== " << endl;
            cout << this->get_player_name() << endl;
            for(int i = 0; i < this->cards_in_hand.size(); i++)
            {
                cout << this->cards_in_hand[i].card_name << "-" << this->cards_in_hand[i].card_suit << " ";
            }
            cout << endl;
            cout << "====================== " << endl;
        }

        int get_player_cards_strength()
        {
            int total_score = 0;
            for(auto card: this->cards_in_hand)
            {
                total_score += card.card_points;
            }
            return total_score;
        }

        void set_player_passed()
        {
            this->player_passed = !this->player_passed;
        }

        bool get_player_passed()
        {
            return this->player_passed;
        }
};

class Board
{
    private:
        int total_cards = 32;
        vector<Card> cards;
        Player* dealer = NULL;
        Player* human_player = NULL;
        int highest_bid = 0;
        Player* highest_bidder = nullptr;

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

    void default_board()
    {
        cout << "=============" << endl;
        cout << "HIGHEST BIDDER ===> " << this->highest_bidder->get_player_name() << endl;
        cout << "HIGHEST BID ===> " << this->highest_bid << endl;
        cout << endl;
        highest_bid = 0;
        highest_bidder = nullptr;

        vector<Player*> players = this->get_players();

        for(Player* player: players)
        {
            if(player->get_player_passed())
                player->set_player_passed();
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
        Player* last_player = (**dealer).right_hand_player;

        //create the new player
        Player* player = new Player(player_name);

        //this new player is going to be the last player now
        //this player's left hand side player will be the dealer
        (*player).left_hand_player = *dealer;

        //and the dealers right hand side player will be now the newly
        //created player
        (**dealer).right_hand_player = player;

        //now newly players right hand side player will be
        //the previous last player who was sitting right next to the dealer
        (*player).right_hand_player = last_player;

        //and the previous last players left hand side player
        //will be now newly created player
        (*last_player).left_hand_player = player;

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

        //for the first time when the program runs
        //this create players function will be executed
        //and it will set the "Human Player" ,which is "You", as the dealer by default
        //and we will keep the reference of this human player throughout the whole program
        //so that we can use it later to compare among the bots and human player
        this->human_player = this->dealer;
    }

    vector<Player*> get_players()
    {
        Player* first_player = this->dealer->left_hand_player;
        Player* second_player = first_player->left_hand_player;
        Player* third_player = second_player->left_hand_player;
        Player* fourth_player = this->dealer;

        vector<Player*> players = {first_player, second_player, third_player, fourth_player};

        return players;
    }

    //before dealing the cards
    //the card will be cut to half by the player right side of
    //the dealer
    void cut_the_cards()
    {
        //there are 32 playing cards
        //which are stacked top of each others
        //we will select the position of the any card
        //then from bottom position to the selected position card
        //we will remove it from the bottom stack and we will put them on the top stack
        //so the following variable will help us to point out that specific card
        //by default we are assigning 0 as a position
        //which is actually the first elements of the 32 size vector
        //that means our first card
        int card_position = 0;

        //getting the right hand side player
        //next to the dealer
        Player* right_hand_player = this->dealer->right_hand_player;

        //check if this player is a bot or human
        if(right_hand_player == this->human_player)
        {
            cout << "Select any number between 1 to 32 to cut the cards in half:\t";
            cin >> card_position;

            //subtracting by 1 because the first element in vector start from 0
            //or the index of any element in vector in (n-1) where n is the position and any integer positive number
            card_position -= 1;
        }
        else
        {
            srand(time(0));
            card_position = rand() % this->cards.size();
        }

        //now we are going to re-arrange the cards
        //this variable will store the bottom of the deck or the first card
        //which is lying at the bottom
        Card* temp_card;


        for(int i = 0; i <= card_position; i++)
        {
            //storing the bottom card
            *temp_card = this->cards[0];
            //pushing the bottom card at the back
            this->cards.push_back(*temp_card);
            //then removing the card from the bottom
            this->cards.erase(this->cards.begin());
        }
    }

    void distribute_the_cards()
    {
        //there will be two distributions
        //in each distribution each players will get 4 cards
        //so in total 4 players will get total 4*4 = 16 cards
        int total_distribution_cards_for_each_player = 4;

        //the distribution will start from the player
        //who is at the left side of the dealer
        //and finally the dealer will get the cards
        Player* first_player = this->dealer->left_hand_player;
        Player* second_player = first_player->left_hand_player;
        Player* third_player = second_player->left_hand_player;
        Player* fourth_player = this->dealer;

        vector<Player*> players = {first_player, second_player, third_player, fourth_player};

        for(int player_index = 0; player_index < players.size(); player_index++)
        {
            for(int i = 0; i < total_distribution_cards_for_each_player; i++)
            {
                players[player_index]->set_cards_in_hand(this->cards.back());
                this->cards.pop_back();
            }
        }
    }

    void bidding()
    {
        cout << "|====================|" << endl;
        cout << "|  16  |  17  |  18  |" << endl;
        cout << "|--------------------|" << endl;
        cout << "|  19  |  20  |  21  |" << endl;
        cout << "|--------------------|" << endl;
        cout << "|  22  |  23  |  24  |" << endl;
        cout << "|--------------------|" << endl;
        cout << "|  25  |  26  |  27  |" << endl;
        cout << "|--------------------|" << endl;
        cout << "|    28   | 0 (Pass) |" << endl;
        cout << "|====================|" << endl;

        //bidding will start from the left side from the dealer
        //so we are setting the current bidder
        Player* current_bidder = this->dealer->left_hand_player;

        //every time a current bidder bids he will stay as a bidder. Suppose the bidder is "A". After "A" bids his left player "B" will get the
        //chance to bid more than "A".
        //when the left person(B) of the current bidder(A) gets his/her(B) turn to bid and bids more then the previous player(A)
        //the previous player (A) will get another chance to bid as same as or more than the other player(B)
        //then the bidding will again go the other player(B). And the player "B" can bids more or pass the bidding.
        //same can be done by the player "A" when he/she(A) gets the chance of another bidding
        //to track these players bidding situation we are going to declare the following variable
        //the variable is going to be a null pointer at first
        //because at first the left player of the dealer will bid. And at the beginning the dealer cannot get the chance to bid after the
        //left side player bids. So we don't need to track the player at first
        Player* current_bidder_right_player = nullptr;

        //boolean variable to track if the bidding is running
        bool is_bidding_running = true;

        //the bidding always start from 16 up to 28
        //so the following int variable is going to store the current bidding store
        //and the initial value is going to be the lowest most bidding score
        int current_bidding_score = 16;

        //time(0) function returns the current time
        //srand() function sets the starting point for producing a series of pseudo-random integers.
        //If srand() is not called, the rand() seed is set as if srand(1) were called at program start.
        //check documentation for in depth knowledge
        srand(time(0));

        //boolean variable to check if the current player is the last bidder
        //in the bidding session
        bool FINAL_BID_RUNNING = false;

        //temporary variable to store a Player data type pointer variable
        //to swap it between current bidder and the other bidder
        Player* temp_bidder = nullptr;

        //int data type variable to check how
        //many players have passed their bidding
        int bid_pass_counter = 0;

        //while loop to run the bidding
        while(is_bidding_running)
        {

            //the following if condition will be executed
            //when the last bidder bids
            //or in other way if any 3 players out of the total 4 players
            //passed their bidding then the following condition will be executed
            if(FINAL_BID_RUNNING)
            {
                //the board variables will be restored to their default state
                //by calling the following method
                this->default_board();
                cout << "Bidding done!" << endl;
                break;
            }

            //the following condition will be executed when all the four players passed their biddings
            //at that time their will be no higher bidder
            if(bid_pass_counter == 4 && this->highest_bidder == nullptr)
            {
                //the board variables will be restored to their default state
                //by calling the following method
                this->default_board();
                cout << "Bidding done!" << endl;
                break;
            }

            cout << "CURRENT BIDDER " << current_bidder->get_player_name() << endl;

            //boolean variable to check if the bot
            //wants to pass bidding
            //or wants to make any bid
            bool bot_wants_to_pass = true;

            //the following condition will check if the current bidder is the actual human player
            //if it is then the following condition will be executed
            //otherwise the else condition will be executed
            if(current_bidder == this->human_player)
            {
                cout << "What is your bidding?\t";
                cin >> current_bidding_score;
                cout << endl;

                //if the current bidding score is larger than the highest bidding
                //then the following if condition will be executed
                if(current_bidding_score > highest_bid)
                {
                    //we are going to store the current bidding score as the highest bidding score
                    this->highest_bid = current_bidding_score;

                    //the following statement will store the current player as the highest bidder
                    this->highest_bidder = current_bidder;

                    //if the following variable is a null pointer then the following if condition will be executed
                    //otherwise the else condition will be executed
                    //the following variable will be a null pointer when the current player is the first bidder
                    //at the beginning of the current bidding session
                    if(current_bidder_right_player == nullptr)
                    {
                        //the current bidder will be stored into the following pointer variable
                        //we will hold this bidder for a while and we will come back to this bidder again
                        //once the left side of this player bids to ask him/her if he/she wants to bid more then the left side player or pass the bidding
                        current_bidder_right_player = current_bidder;

                        //we are going to go the left side player so that we can
                        //as the player for bidding
                        //the following statement is going to change the current bidder
                        current_bidder = current_bidder->left_hand_player;
                    }
                    else
                    {
                        //suppose the first bidder was player "A" and second bidder was player "B"
                        //when player "A" was bidding he was the "current bidder"
                        //then when the player "B" was bidding he/she was the "current bidder" and
                        //at the time of player "B" bidding, the player "A" became the "waiting bidder" who would get another chance to bid
                        //once player "B" would be done
                        //so at that point player "A" would be the "current bidder right player"
                        //after player "B" was done with bidding we need to make the player "A" as a current bidder
                        //and then the player "B" will be the "waiting bidder" and he/she will get another chance to bid and then player "B" will be
                        //the "current bidder" again and son on
                        //this bidding between player "A" and "B" will be running until one of them passed bidding
                        //that's why the following 3 statements are there
                        //we are actually swapping the "current bidder" and the "waiting bidder"
                        temp_bidder = current_bidder;
                        current_bidder = current_bidder_right_player;
                        current_bidder_right_player = temp_bidder;
                    }

                    //suppose the players are sit like the following pattern
                    //the arrows indicate how the bidding flow goes
                    //==========
                    //
                    //Player A      ->    Player B
                    //  ^                   |
                    //  |                   v
                    //Player C      <-   Player D (Card Dealer)
                    //
                    //==========
                    //Suppose player 'D' is the dealer
                    //so the first bidder will be player 'C'
                    //at this time there will be no "waiting bidder"
                    //suppose player 'C' bids a certain number
                    //now the player 'C' will be the 'waiting bidder' and player 'A' is going to be the next or current bidder
                    //suppose the player 'A' pass the bidding
                    //now the waiting bidder is still the player 'C'
                    //and current bidding will go the player 'D' and player 'D' is now the 'current bidder'
                    //suppose the player 'D' bids more than player 'C'
                    //now we will swap the 'current bidder' and the 'waiting bidder'
                    //now the player 'D' is going to be the 'waiting bidder' and player 'C' is going to be the 'current bidder'
                    //now suppose the 'current bidder' player 'C' pass the bidding
                    //at this point player 'D' is still the 'waiting bidder'
                    //now we need to look for the next bidder who has not passed bidding
                    //that player is going to be our 'current bidder'
                    //but there is only one player left who has not passed bidding and
                    //that player is going to be the player 'D'
                    //at this point the player 'D' is going to be the 'current bidder' and also the 'waiting bidder'
                    //that means we reached to our final bidder

                    //the following condition is going to check if the 'current bidder' and the 'waiting bidder' is the same player
                    //if it is then the final bid is done
                    //and we are going to change the following boolean variable to true
                    if(current_bidder == current_bidder_right_player)
                    {
                        FINAL_BID_RUNNING = true;
                    }
                }
                //this following 'else if' condition will be executed if the human player pass bidding
                else if(current_bidding_score == 0)
                {
                    //we are going to set the member data inside the Player instance
                    //to true to mean that that specific player has passed bidding
                    current_bidder->set_player_passed();

                    //increasing the number of the players that passed
                    //bidding by 1 (one)
                    bid_pass_counter++;

                    //if the current bidder is the first bidder
                    //or if there is no 'waiting' bidder
                    //then the following if condition will be executed

                    //suppose the players are sit like the following pattern
                    //the arrows indicate how the bidding flow goes
                    //==========
                    //
                    //Player A      ->    Player B
                    //  ^                   |
                    //  |                   v
                    //Player C      <-   Player D (Card Dealer)
                    //
                    //==========
                    //Suppose player 'D' is the dealer
                    //at first player 'C' is going to be the 'current bidder'
                    //so there is no 'waiting bidder'
                    //suppose player 'C' passed bidding
                    //still 'waiting bidder' is none
                    //the bidding will go the player 'A'
                    //now player 'A' is the current bidder
                    //if player 'A' pass the bidding too then we need to go the player 'B'
                    //because there is no 'waiting bidder'
                    //this is exactly what is going on inside the following if condition
                    if(current_bidder_right_player == nullptr)
                    {
                        //if the first bidder pass then the current bidding
                        //will go the left player
                        current_bidder = current_bidder->left_hand_player;
                    }
                    else
                    {
                        //this body of the 'else' condition will find our who needs to bid next
                        //suppose the players are sit like the following pattern
                        //the arrows indicate how the bidding flow goes
                        //==========
                        //
                        //Player A      ->    Player B
                        //  ^                   |
                        //  |                   v
                        //Player C      <-   Player D (Card Dealer)
                        //
                        //==========
                        //Suppose player 'D' is the dealer
                        //so the first bidder is player 'C' or we may say that the 'current bidder'
                        //the 'waiting bidder' is none at this point
                        //suppose player 'C' bids a certain number and player 'C' going to be the 'highest bidder' for now
                        //now the 'waiting bidder' is going to be the player 'C'
                        //'current bidder' is now player 'A'
                        //suppose the player 'A' pass the bidding
                        //so right now we need to go the player 'B'
                        //to do that we are going to execute the following statements

                        //first we are going to set the current bidder as the 'highest bidder'
                        //this is for a temporary time to track down the expected bidder
                        current_bidder = this->highest_bidder;

                        //now we are going inside an infinite while loop until we get the
                        //expected bidder
                        while(true)
                        {
                            //when the while loop runs for the first time
                            //we are going to the left player of the 'highest bidder'
                            //because we already know that 'highest bidder' has already bid
                            //so we don't need set the 'highest bidder' as the 'current bidder'
                            //this time we will check if the 'current bidder' passed bidding
                            //if the current bidder passed bidding then we will run the while loop again
                            //and go the next player
                            //the following if condition will do that for us
                            //and if the condition is true then the while loop will break

                            //=======Example 1 =====
                            //consider the previously discussed bidding pattern
                            //where player 'C' bid but the player 'A' passed and now we need to make the player 'B' as the current bidder
                            //at this time the 'highest bidder' is the player 'C'
                            //first time when the while loop runs we will set player 'A' as the 'current player'
                            //by the following variable re-assigning
                            //then we will check by the 'if condition' if the player 'A' passed bidding
                            //if player 'A' passed bidding then the while loop run again and this time we are going
                            //to set the player 'B' as the 'current player'
                            //and this time we will see that the player never passed before
                            //so the while loop condition will break
                            //player 'B' will be our 'current bidder'
                            //and the 'highest bidder' which was player 'C' will be the 'waiting bidder'


                            //======== Example 2 =======
                            //there can be another situation where suppose
                            //instead of passing the bidding, player 'A' makes a certain number of bid
                            //and then the bidding will be go the player 'C' again
                            //and this time suppose the player 'C' passed the bidding
                            //at this time player 'C' will be the highest bidder
                            //so the current while loop will run one time and the 'current bidder' will be the player 'B'
                            //making the player 'A' as a 'waiting bidder'
                            current_bidder = current_bidder->left_hand_player;

                            if(!current_bidder->get_player_passed())
                            {
                                break;
                            }
                        }

                        //the following statement will set the 'highest bidder' as the 'waiting bidder'
                        current_bidder_right_player = this->highest_bidder;

                        //suppose the players are sit like the following pattern
                        //the arrows indicate how the bidding flow goes
                        //==========
                        //
                        //Player A      ->    Player B
                        //  ^                   |
                        //  |                   v
                        //Player C      <-   Player D (Card Dealer)
                        //
                        //==========
                        //Suppose player 'D' is the dealer
                        //so the first bidder will be player 'C'
                        //at this time there will be no "waiting bidder"
                        //suppose player 'C' bids a certain number
                        //now the player 'C' will be the 'waiting bidder' and player 'A' is going to be the next or current bidder
                        //suppose the player 'A' pass the bidding
                        //now the waiting bidder is still the player 'C'
                        //and current bidding will go the player 'D' and player 'D' is now the 'current bidder'
                        //suppose the player 'D' bids more than player 'C'
                        //now we will swap the 'current bidder' and the 'waiting bidder'
                        //now the player 'D' is going to be the 'waiting bidder' and player 'C' is going to be the 'current bidder'
                        //now suppose the 'current bidder' player 'C' pass the bidding
                        //at this point player 'D' is still the 'waiting bidder'
                        //now we need to look for the next bidder who has not passed bidding
                        //that player is going to be our 'current bidder'
                        //but there is only one player left who has not passed bidding and
                        //that player is going to be the player 'D'
                        //at this point the player 'D' is going to be the 'current bidder' and also the 'waiting bidder'
                        //that means we reached to our final bidder

                        //the following condition is going to check if the 'current bidder' and the 'waiting bidder' is the same player
                        //if it is then the final bid is done
                        //and we are going to change the following boolean variable to true
                        if(current_bidder == current_bidder_right_player)
                        {
                            FINAL_BID_RUNNING = true;
                        }
                    }
                }
                else
                {
                    cout << "Bidding should be greater than current highest bidding. Input 0 (Zero) to pass, Try again." << endl;
                    continue;
                }
            }
            else
            {
                //rand() % n will result any integer number between 0 to (n-1)
                //the following variable will be use to make a decision about bot
                //if the bot wants to pass the bidding or make any bidding
                int temp_decision = rand() % 2;

                if(temp_decision == 0)
                    bot_wants_to_pass = false;
                else
                    bot_wants_to_pass = true;

                //if bot wants to pass then the following 'if' condition will be executed
                //otherwise the 'else' condition will be executed
                if(bot_wants_to_pass)
                {
                    cout << current_bidder->get_player_name() << " passes bidding!" << endl;

                    //we are going to set the member data inside the Player instance
                    //to true to mean that that specific player has passed bidding
                    current_bidder->set_player_passed();

                    //increasing the number of the players that passed
                    //bidding by 1 (one)
                    bid_pass_counter++;

                    //if the current bidder is the first bidder
                    //or if there is no 'waiting' bidder
                    //then the following if condition will be executed

                    //suppose the players are sit like the following pattern
                    //the arrows indicate how the bidding flow goes
                    //==========
                    //
                    //Player A      ->    Player B
                    //  ^                   |
                    //  |                   v
                    //Player C      <-   Player D (Card Dealer)
                    //
                    //==========
                    //Suppose player 'D' is the dealer
                    //at first player 'C' is going to be the 'current bidder'
                    //so there is no 'waiting bidder'
                    //suppose player 'C' passed bidding
                    //still 'waiting bidder' is none
                    //the bidding will go the player 'A'
                    //now player 'A' is the current bidder
                    //if player 'A' pass the bidding too then we need to go the player 'B'
                    //because there is no 'waiting bidder'
                    //this is exactly what is going on inside the following if condition
                    if(current_bidder_right_player == nullptr)
                    {
                        //if the first bidder pass then the current bidding
                        //will go the left player
                        current_bidder = current_bidder->left_hand_player;
                    }
                    else
                    {
                        //this body of the 'else' condition will find our who needs to bid next
                        //suppose the players are sit like the following pattern
                        //the arrows indicate how the bidding flow goes
                        //==========
                        //
                        //Player A      ->    Player B
                        //  ^                   |
                        //  |                   v
                        //Player C      <-   Player D (Card Dealer)
                        //
                        //==========
                        //Suppose player 'D' is the dealer
                        //so the first bidder is player 'C' or we may say that the 'current bidder'
                        //the 'waiting bidder' is none at this point
                        //suppose player 'C' bids a certain number and player 'C' going to be the 'highest bidder' for now
                        //now the 'waiting bidder' is going to be the player 'C'
                        //'current bidder' is now player 'A'
                        //suppose the player 'A' pass the bidding
                        //so right now we need to go the player 'B'
                        //to do that we are going to execute the following statements

                        //first we are going to set the current bidder as the 'highest bidder'
                        //this is for a temporary time to track down the expected bidder
                        current_bidder = this->highest_bidder;

                        //now we are going inside an infinite while loop until we get the
                        //expected bidder
                        while(true)
                        {
                            //when the while loop runs for the first time
                            //we are going to the left player of the 'highest bidder'
                            //because we already know that 'highest bidder' has already bid
                            //so we don't need set the 'highest bidder' as the 'current bidder'
                            //this time we will check if the 'current bidder' passed bidding
                            //if the current bidder passed bidding then we will run the while loop again
                            //and go the next player
                            //the following if condition will do that for us
                            //and if the condition is true then the while loop will break

                            //=======Example 1 =====
                            //consider the previously discussed bidding pattern
                            //where player 'C' bid but the player 'A' passed and now we need to make the player 'B' as the current bidder
                            //at this time the 'highest bidder' is the player 'C'
                            //first time when the while loop runs we will set player 'A' as the 'current player'
                            //by the following variable re-assigning
                            //then we will check by the 'if condition' if the player 'A' passed bidding
                            //if player 'A' passed bidding then the while loop run again and this time we are going
                            //to set the player 'B' as the 'current player'
                            //and this time we will see that the player never passed before
                            //so the while loop condition will break
                            //player 'B' will be our 'current bidder'
                            //and the 'highest bidder' which was player 'C' will be the 'waiting bidder'


                            //======== Example 2 =======
                            //there can be another situation where suppose
                            //instead of passing the bidding, player 'A' makes a certain number of bid
                            //and then the bidding will be go the player 'C' again
                            //and this time suppose the player 'C' passed the bidding
                            //at this time player 'C' will be the highest bidder
                            //so the current while loop will run one time and the 'current bidder' will be the player 'B'
                            //making the player 'A' as a 'waiting bidder'

                            current_bidder = current_bidder->left_hand_player;
                            if(!current_bidder->get_player_passed())
                            {
                                break;
                            }
                        }

                        //the following statement will set the 'highest bidder' as the 'waiting bidder'
                        current_bidder_right_player = this->highest_bidder;

                        //suppose the players are sit like the following pattern
                        //the arrows indicate how the bidding flow goes
                        //==========
                        //
                        //Player A      ->    Player B
                        //  ^                   |
                        //  |                   v
                        //Player C      <-   Player D (Card Dealer)
                        //
                        //==========
                        //Suppose player 'D' is the dealer
                        //so the first bidder will be player 'C'
                        //at this time there will be no "waiting bidder"
                        //suppose player 'C' bids a certain number
                        //now the player 'C' will be the 'waiting bidder' and player 'A' is going to be the next or current bidder
                        //suppose the player 'A' pass the bidding
                        //now the waiting bidder is still the player 'C'
                        //and current bidding will go the player 'D' and player 'D' is now the 'current bidder'
                        //suppose the player 'D' bids more than player 'C'
                        //now we will swap the 'current bidder' and the 'waiting bidder'
                        //now the player 'D' is going to be the 'waiting bidder' and player 'C' is going to be the 'current bidder'
                        //now suppose the 'current bidder' player 'C' pass the bidding
                        //at this point player 'D' is still the 'waiting bidder'
                        //now we need to look for the next bidder who has not passed bidding
                        //that player is going to be our 'current bidder'
                        //but there is only one player left who has not passed bidding and
                        //that player is going to be the player 'D'
                        //at this point the player 'D' is going to be the 'current bidder' and also the 'waiting bidder'
                        //that means we reached to our final bidder

                        //the following condition is going to check if the 'current bidder' and the 'waiting bidder' is the same player
                        //if it is then the final bid is done
                        //and we are going to change the following boolean variable to true
                        if(current_bidder == current_bidder_right_player)
                        {
                            FINAL_BID_RUNNING = true;
                        }
                    }


                }
                else if(!bot_wants_to_pass)
                {
                    //use this variable data later
                    int player_cards_strenght = current_bidder->get_player_cards_strength();
                    //////

                    current_bidding_score = current_bidding_score + rand() % 3;

                    if(current_bidding_score > 28)
                        current_bidding_score = 28;

                    cout << current_bidder->get_player_name() << " bids " << current_bidding_score << endl;

                    //we are going to store the current bidding score as the highest bidding score
                    this->highest_bid = current_bidding_score;

                    //the following statement will store the current player as the highest bidder
                    this->highest_bidder = current_bidder;


                    //if the following variable is a null pointer then the following if condition will be executed
                    //otherwise the else condition will be executed
                    //the following variable will be a null pointer when the current player is the first bidder
                    //at the beginning of the current bidding session
                    if(current_bidder_right_player == nullptr)
                    {

                        //the current bidder will be stored into the following pointer variable
                        //we will hold this bidder for a while and we will come back to this bidder again
                        //once the left side of this player bids to ask him/her if he/she wants to bid more then the left side player or pass the bidding
                        current_bidder_right_player = current_bidder;

                        //we are going to go the left side player so that we can
                        //as the player for bidding
                        //the following statement is going to change the current bidder
                        current_bidder = current_bidder->left_hand_player;
                    }
                    else
                    {

                        //suppose the first bidder was player "A" and second bidder was player "B"
                        //when player "A" was bidding he was the "current bidder"
                        //then when the player "B" was bidding he/she was the "current bidder" and
                        //at the time of player "B" bidding, the player "A" became the "waiting bidder" who would get another chance to bid
                        //once player "B" would be done
                        //so at that point player "A" would be the "current bidder right player"
                        //after player "B" was done with bidding we need to make the player "A" as a current bidder
                        //and then the player "B" will be the "waiting bidder" and he/she will get another chance to bid and then player "B" will be
                        //the "current bidder" again and son on
                        //this bidding between player "A" and "B" will be running until one of them passed bidding
                        //that's why the following 3 statements are there
                        //we are actually swapping the "current bidder" and the "waiting bidder"
                        temp_bidder = current_bidder;
                        current_bidder = current_bidder_right_player;
                        current_bidder_right_player = temp_bidder;
                    }

                    //suppose the players are sit like the following pattern
                    //the arrows indicate how the bidding flow goes
                    //==========
                    //
                    //Player A      ->    Player B
                    //  ^                   |
                    //  |                   v
                    //Player C      <-   Player D (Card Dealer)
                    //
                    //==========
                    //Suppose player 'D' is the dealer
                    //so the first bidder will be player 'C'
                    //at this time there will be no "waiting bidder"
                    //suppose player 'C' bids a certain number
                    //now the player 'C' will be the 'waiting bidder' and player 'A' is going to be the next or current bidder
                    //suppose the player 'A' pass the bidding
                    //now the waiting bidder is still the player 'C'
                    //and current bidding will go the player 'D' and player 'D' is now the 'current bidder'
                    //suppose the player 'D' bids more than player 'C'
                    //now we will swap the 'current bidder' and the 'waiting bidder'
                    //now the player 'D' is going to be the 'waiting bidder' and player 'C' is going to be the 'current bidder'
                    //now suppose the 'current bidder' player 'C' pass the bidding
                    //at this point player 'D' is still the 'waiting bidder'
                    //now we need to look for the next bidder who has not passed bidding
                    //that player is going to be our 'current bidder'
                    //but there is only one player left who has not passed bidding and
                    //that player is going to be the player 'D'
                    //at this point the player 'D' is going to be the 'current bidder' and also the 'waiting bidder'
                    //that means we reached to our final bidder

                    //the following condition is going to check if the 'current bidder' and the 'waiting bidder' is the same player
                    //if it is then the final bid is done
                    //and we are going to change the following boolean variable to true
                    if(current_bidder == current_bidder_right_player)
                    {
                        FINAL_BID_RUNNING = true;
                    }

                }
            }

            cout << endl;
            system("pause");
            cout << endl;
        }


    }

};









int main()
{




    Board board = Board();

    board.create_players();



    board.shuffle_cards();
    board.cut_the_cards();

    board.get_cards();

    board.distribute_the_cards();


    vector<Player*> players = board.get_players();

    players[0]->show_cards_in_hand();
    players[1]->show_cards_in_hand();
    players[2]->show_cards_in_hand();
    players[3]->show_cards_in_hand();

    while(true)
        board.bidding();

    return 0;
}





