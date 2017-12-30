/*=============================================================================================================
//Author:	Holly Eaton
//
//Program:	Twenty-One
//
//Dev Env:	Visual Studio
//
//Description: A program that plays the game of Blackjack.
//
=======================================================================================================================*/

//Header file
#include <iostream>
#include <cstdlib>	//(for rand)
#include <ctime>	//(for time)
#include <string>
using namespace std;

//constant values
const int DECK_SIZE = 52;
const int HAND_SIZE = 10;

//function prototypes.
void initializeDeck(int deck[]);
int getRandomNumber(int low, int high);
void shuffle(int deck[], int size);
void showCard(int card);
void showCards(const int cards[], int numCards, bool hideFirstCard);
int cardValue(int card);
int getTopCard(int deck[]);
void addToHand(int hand[], int cardToAdd);
int getHandValue(const int hand[]);
void showBothPlayersCards(const int player[], const int dealer[], int size, bool hideFirstCard);
bool wantsCard();
bool playerWantsToContinue();
bool playOneHand(int deck[], int player[], int dealer[]);
void clearScreen();
void whoWonWhy(bool winner, const int player[], const int dealer[]);
void reinitializeHand(int hand[]);
//=======================================================================================================================

int main ()
{
	int deck[DECK_SIZE];
	bool winner;
	int playerHand[HAND_SIZE] = {0};
	int dealerHand[HAND_SIZE] = {0};

	initializeDeck(deck);
	shuffle(deck, DECK_SIZE);

	do
	{
		winner=playOneHand(deck, playerHand, dealerHand);
		whoWonWhy(winner, playerHand, dealerHand);

	}
	while (((deck[DECK_SIZE-15]!=0) && playerWantsToContinue()));
	cout<<endl<<"Thanks for playing!";


	//keep console window open when ran in GUI
	cin.ignore();
	cin.get();

	return 0;
}
//======================================================================================================================
/*======================================================================================================================
// Function: initializeDataset 
// Description: initialize all elements of given array to 2*index
// 
// Argument List: 
//	deck[] - (O) - The array to initialize 
// 
=========================================================================================================================*/
//Create an array to hold the deck:
void initializeDeck(int deck[]);

void initializeDeck(int deck[])
{
	int index;

	for(index = 0; index<13; index++)
		deck[index]=index+101;

	for(index = 13; index<26; index++)
		deck[index]=index+(201-13);

	for(index = 26; index<39; index++)
		deck[index]=index+(301-26);

	for(index = 39; index<52; index++)
		deck[index]=index+(401-39);
}
/*======================================================================================================================
// Function:    getRandomNumber
// Description: returns a random number between given low and high
//	values, inclusive. 
//	Note: include cstdlib (for rand) and ctime (for time).
// Arguments:
//	low (I) - The lowest number to be generated
//	high (I) - The highest number to be generated (must be > low)
// Return value:
//	A random number between low and high (inclusive)
=========================================================================================================================*/
int getRandomNumber(int low, int high) {
	static bool firstTime=true;
	int randNum;

	//if first time called, seed random number generator
	if (firstTime) {
		srand( static_cast<unsigned int>(time(NULL)) );
		firstTime=false;
	}

	//generate random number between given low and high values (inclusive)
	randNum = rand() % (high-low+1) + low;

	return randNum;
}
/*======================================================================================================================
// Function: shuffle 
// Description: randomizes the deck
// 
// Argument List: 
//	deck[] - (I/O) - The deck to shuffle 
//	size - (I) - the size of the array 
// 
=========================================================================================================================*/ 
void shuffle(int deck[], int size)
{
	int card1;
	int card2;
	int tempCard;
	for(int count=1; count<=500; count++)
	{
		card1=getRandomNumber(0, (size-1));
		card2=getRandomNumber(0, (size-1));
		tempCard=deck[card2];
		deck[card2]=deck[card1];
		deck[card1]=tempCard;
	}
}
/*======================================================================================================================
// Function: showCard 
// Description: prints the given card's rank and suit
// 
// Argument List: 
//	card - (I) - The card to print 
// 
=========================================================================================================================*/ 
void showCard(int card)
{
	cout<<static_cast<char>((card/100)+2);
	if ((card%100>1) && (card%100<11))
		cout<<card%100<<" ";
	else
		switch (card%100)
	{
		case 1:
			cout<<"A ";
			break;
		case 11:
			cout<<"J ";
			break;
		case 12:
			cout<<"Q ";
			break;
		case 13:
			cout<<"K ";
			break;
		default:
			cout<<"Something is really screwed up because you should never get to the default in this facecard switch statement";
			break;
	}
}
/*======================================================================================================================
// Function: showCards 
// Description: prints cards in hand to screen
// 
// Argument List: 
//	cards[] - (I) - The hand to print 
//	numCards - (I) - the size of the hand
// 
=========================================================================================================================*/ 
void showCards(const int cards[], int numCards, bool hideFirstCard)
{
	if (hideFirstCard && numCards==2)
	{
		showCard(cards[0]);
		cout<<" ** ";
	}
	else
	{
		for(int index=0; index<numCards; index++)
		{
			if (cards[index]==0)
				return;
			else
			{
				showCard(cards[index]);
				cout<<" ";
			}

		}
	}
}
/*======================================================================================================================
// Function: cardValue 
// Description: returns the face value of the card
// 
// Argument List: 
//	card - (I) - The card whos face value is to be determined 
// Return value:
//	The number used to play the game.
// 
=========================================================================================================================*/ 
int cardValue(int card)
{
	switch (card%100)
	{
	case 1:
		return 11;
		break;
	case 11:
	case 12:
	case 13:
		return 10;
		break;
	default:
		return card%100;
	}
}
/*======================================================================================================================
// Function: getTopCard 
// Description: gets the next card from the first index that is not zero
// 
// Argument List: 
//	deck[] - (I/O) - The deck to take the card off and replace it with a zero. 
// Return value:
//	The value of the top card of the deck.
// 
=========================================================================================================================*/ 
int getTopCard(int deck[])
{
	int index=0, card;

	//Test to see if the index value is 0 if not take it.
	do
	{
		if (deck[index] == 0 )
			index++;
		else
		{
			card = deck[index];
			deck[index] = 0;
			index++;
			return card;	
		}
	}
	while (deck[DECK_SIZE-1] !=0);
	return 0;
}
/*======================================================================================================================
// Function: addToHand 
// Description: Adds the given card to an array representing a player hand.
// 
// Argument List: 
//	hand[] - (I/O) - The hand that is adding a card 
//	cardToAdd - (I) - The card whos face value is being added to the hand
// 
=========================================================================================================================*/ 
void addToHand(int hand[], int cardToAdd)
{
	int index=0;
	while(hand[index] !=0)
		index++;
	hand[index]= cardToAdd;
}
/*======================================================================================================================
// Function: getHandValue 
// Description: Computes and returns the value of the given hand.
// 
// Argument List: 
//	hand[] - (I/O) - The hand that is being evaluated.
// 
=========================================================================================================================*/ 
int getHandValue(const int hand[])
{
	int index = 0, value = 0, numAces=0;

	do
	{
		value += cardValue(hand[index]);
		if(cardValue(hand[index])==11)
			++numAces;
		index++;
	}
	while (hand[index] !=0);

	for(index=0; index<numAces; index++)
		if (value>21) value -=10;
	return value;
}
/*======================================================================================================================
// Function: showBothPlayersCards 
// Description: Displays all hands in play.
// 
// Argument List: 
//	player[] - (I) - player's hand.
//	dealer[] - (I) - dealer's hand.
//	size - (I) - size of the hand.
//	hideFirstCard - (I) - True - hide the dealer's first card
//						  False - show the first card
// 
=========================================================================================================================*/ 
void showBothPlayersCards(const int player[], const int dealer[], int size, bool hideFirstCard)
{
	cout<<"Player's hand is: ";
	showCards(player, HAND_SIZE, false);
	cout<<"   "<<getHandValue(player)<<endl;

	cout<<"Dealer's hand is: ";
	showCards(dealer, size, hideFirstCard);
	cout<<endl;
}
/*======================================================================================================================
// Function: wantsCard 
// Description: Asks player if they want a card.
// 
// Argument List: 
//	No arguments
// Return value:	True if they want one.
//					False if they don't.			
// 
=========================================================================================================================*/ 
bool wantsCard()
{
	char answer;

	cout<<"\nWould you like another card (Y/N)? ";
	cin>>answer;
	while (toupper(answer) != 'Y')
		return false;
	return true;
}
/*======================================================================================================================
// Function: playerWantsToContinue 
// Description: Asks player if they want to play another hand.
// 
// Argument List: 
//	No arguments
// Return value:	True if they want to play.
//					False if they don't.			
// 
=========================================================================================================================*/
bool playerWantsToContinue()
{
	char answer;

	cout<<"\nWould you like you like to play another hand (Y/N)? ";
	cin>>answer;
	cout<<endl;
	while (toupper(answer) != 'N')
		return true;
	return false;
}
/*======================================================================================================================
// Function: playOneHand 
// Description: Plays a hand of Blackjack.
// 
// Argument List: 
//	deck[] - (I/O) - The shuffled deck for the game.
//
// Return value:	True player wins.
//					False dealer wins.			
// 
=========================================================================================================================*/
bool playOneHand(int deck[], int player[], int dealer[])
{
	//must reinitialize array to play subsequent hands
	reinitializeHand(player);
	reinitializeHand(dealer);

	//Deal a hand of two cards to both players alternating
	addToHand(player, getTopCard(deck));
	addToHand(dealer, getTopCard(deck));
	addToHand(player, getTopCard(deck));
	addToHand(dealer, getTopCard(deck));

	//Show the hands
	clearScreen();
	showBothPlayersCards(player, dealer, 2, true);

	if(getHandValue(dealer)==21)
	{
		showBothPlayersCards(player, dealer, 2, false);
		return false ;	
	}
	if (getHandValue(player)==21)
	{
		clearScreen();
		showBothPlayersCards(player, dealer, 2, false);
		return true;
	}

	while (getHandValue(player)<=21 && wantsCard())
	{
		addToHand(player, getTopCard(deck));
		showBothPlayersCards(player, dealer, 2, true);

	}
	if (getHandValue(player)>21)	
		return false;

	while (getHandValue(dealer)<17)
	{
		clearScreen();
		addToHand(dealer, getTopCard(deck));
		showBothPlayersCards(player, dealer, HAND_SIZE, false);
	}
	clearScreen();
	showBothPlayersCards(player, dealer, HAND_SIZE, false);

	if (getHandValue(dealer)>21)
		return true;

	else if (getHandValue(player)>getHandValue(dealer))
		return true;
	else if (getHandValue(player)<getHandValue(dealer))
		return false;
	else 
		return false;
}
/*======================================================================================================================
// Function: clearScreen 
// Description: Clears the screen

=========================================================================================================================*/
void clearScreen()
{
	cout<<string(20, '\n');
}
/*======================================================================================================================
// Function: whoWonWhy 
// Description: Who won the hand and why.
// 
// Argument List: 
//	winner - (I) - true the player won and false the dealer won the hand in playOneHand.
//
=========================================================================================================================*/
void whoWonWhy(bool winner, const int player[], const int dealer[])
{
	if(winner)
	{
		if (getHandValue(player)==21)
			cout<<endl<<"Player has Black Jack! Player Wins!"<<endl;
		else if (getHandValue(dealer)>21)
			cout<<endl<<"Dealer Busted! Player Wins!";
		else if (getHandValue(player) > getHandValue(dealer))
			cout<<endl<<"Player has "<<getHandValue(player)<<" dealer has "<<getHandValue(dealer)<<" Player Wins!"<<endl;
	}
	else 
	{
		if (getHandValue(dealer)==21 && dealer[2]==0)
			cout<<"Dealer has Black Jack! Dealer Wins!"<<endl;
		else if (getHandValue(player)>21)
			cout<<endl<<"Player Busted! Dealer Wins!";
		else if (getHandValue(dealer) > getHandValue(player))
			cout<<endl<<"Dealer has "<<getHandValue(dealer)<<" Player has "<<getHandValue(player)<<" Dealer Wins!"<<endl;
		else if (getHandValue(dealer) == getHandValue(player))
			cout<<endl<<"Both players have "<<getHandValue(player)<<" Dealer Wins!"<<endl;
	}
}
/*======================================================================================================================
// Function: reinitializeHand 
// Description: Resets all player hands.
// 
// Argument List: 
//	hand[] - (O) - the to be reset.
//
=========================================================================================================================*/
void reinitializeHand(int hand[])
{
	for(int index=0; index<HAND_SIZE; index++)
		hand[index]=0;
}