#include <algorithm>
#include <ctime>
#include <iostream>
#include <iterator>
#include <map>
#include <set>
#include <string>
#include <vector>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
using namespace std;

void Delay(int t){
#ifdef WINDWS
	Sleep(t);
#else
	usleep(1000*t);
#endif
}
// append to wallhand from tile 4time
void AppendToWallHand(map<string, string> tiles, vector<string> &wallhand) {
  for (map<string, string>::iterator tile = tiles.begin(); tile != tiles.end();
       tile++)
    for (int i = 0; i < 4; ++i)
      wallhand.push_back(tile->first);
};

// draw card from wallhand to playerhand
void DrawCard(vector<string> &wallhand, vector<string> &playerhand) {
  playerhand.push_back(wallhand.front());
  wallhand.erase(wallhand.begin());
}

// sort playerhand by sortby
void Sort(vector<string> sortby, vector<string> &playerhand) {
  vector<string> temphand;
  for (vector<string>::iterator s = sortby.begin(); s != sortby.end(); s++)
    if (find(playerhand.begin(), playerhand.end(), *s) != playerhand.end())
      for (int j = 0; j < count(playerhand.begin(), playerhand.end(), *s); j++)
        temphand.push_back(*s);
  playerhand = temphand;
}

// set bookmaker and drawcard sort
void SetUp(bool player1bookmaker, bool player2bookmaker,
								map<string,string> tile, vector<string> &wallhand,
							 	vector<string> &player1hand,	vector<string> &player2hand,
								vector<string> sortby){
  // append tile to wallhand and shuffle
  AppendToWallHand(tile, wallhand);
  random_shuffle(wallhand.begin(), wallhand.end());
	if (player1bookmaker){
		// playerdrawcard 2 tile 3 time (6tile)
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 2; j++)
				DrawCard(wallhand, player1hand);
			for (int j = 0; j < 2; j++)
				DrawCard(wallhand, player2hand);
		}
		// playerdraw the 7th tile
		DrawCard(wallhand, player1hand);
		DrawCard(wallhand, player2hand);
		// player1 draw the 8th card
		DrawCard(wallhand, player1hand);
		// sort all playerhand
		Sort(sortby, player1hand);
		Sort(sortby, player2hand);
	}else if (player2bookmaker){
		// playerdrawcard 2 tile 3 time (6tile)
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 2; j++)
				DrawCard(wallhand, player2hand);
			for (int j = 0; j < 2; j++)
				DrawCard(wallhand, player1hand);
		}
		// playerdraw the 7th tile
		DrawCard(wallhand, player2hand);
		DrawCard(wallhand, player1hand);
		// player1 draw the 8th card
		DrawCard(wallhand, player2hand);
		// sort all playerhand
		Sort(sortby, player2hand);
		Sort(sortby, player1hand);
	}
}

// PrintAll class
class PrintAll {
private:
	// declare
  string              player1handname;
  string              player2handname;
  string              wallname;
  string              rivername;
  string              player1meldhandname;
  string              player2meldhandname;
  map<string, string> tile;
  void                PrintMeldHand(int                    whitchplayermeldhand,
                                    vector<vector<string>> meldhand) {
    vector<string> tempmeld;
    switch (whitchplayermeldhand) {
    case 1:
      cout << this->player1meldhandname;
      break;
    case 2:
      cout << this->player2meldhandname;
      break;
    }
    cout << ':';
    for (vector<vector<string>>::iterator meld = meldhand.begin();
         meld != meldhand.end(); meld++) {
      cout << "{ ";
      tempmeld = *meld;
      for (vector<string>::iterator m = tempmeld.begin(); m != tempmeld.end();
           m++)
        cout << this->tile[*m] << ' ';
      cout << '}';
    }
    cout << endl << endl;
  };
public:
  void PrintAll_SetNameAndMap(string player1handname, string player2handname,
                              string wallname, string rivername,
                              string              player1meldhandname,
                              string              player2meldhandname,
                              map<string, string> tile) {
    this->player1handname = player1handname;
    this->player2handname = player2handname;
    this->wallname = wallname;
    this->rivername = rivername;
    this->player1meldhandname = player1meldhandname;
    this->player2meldhandname = player2meldhandname;
    this->tile = tile;
  };
  void PrintHand(int whitchplayer, vector<string> hand, bool shownumber) {
    switch (whitchplayer) { // -1 = river, 0 = wall, 1 = player1, 2 = player2
    case -1:
      cout << this->rivername;
      break;
    case 0:
      cout << this->wallname;
      break;
    case 1:
      cout << this->player1handname;
      break;
    case 2:
      cout << this->player2handname;
      break;
    }
    cout << ':';
    if (shownumber)
      for (int i = 0; i < hand.size(); i++)
        cout << i + 1 << "." << this->tile[hand[i]] << "  ";
    else {
      for (vector<string>::iterator target = hand.begin(); target != hand.end();
           target++)
        cout << this->tile[*target] << ' ';
    }
    cout << endl << endl;
  };
  void PrintAll_Print(vector<string> player1hand, vector<string> player2hand,
                      vector<string> wallhand, vector<string> riverhand,
                      vector<vector<string>> player1meldhand,
                      vector<vector<string>> player2meldhand) {
		cout << "============================================================" << endl;
    // print wall
    PrintHand(0, wallhand, false);
		cout << "------------------------------------------------------------" << endl;
    // print player1
    PrintHand(1, player1hand, false);
    PrintMeldHand(1, player1meldhand);
		cout << "------------------------------------------------------------" << endl;
    PrintHand(-1, riverhand, false);
		cout << "------------------------------------------------------------" << endl;
    PrintMeldHand(2, player2meldhand);
    PrintHand(2, player2hand, false);
		cout << "============================================================" << endl;
  };
};

// CheckWin  class
class CheckWin {
private:
  vector<string> tile;
  vector<string> hand;
  // probabley eyes
  vector<string> ProbablyEyes() {
    vector<string> temphand;
    for (vector<string>::iterator t = this->tile.begin(); t != this->tile.end();
         t++)
      if (find(this->hand.begin(), this->hand.end(), *t) != this->hand.end() &&
          count(this->hand.begin(), this->hand.end(), *t) >= 2)
        temphand.push_back(*t);
    return temphand;
  };
  // check meld
  bool CheckMeld(string a, string b, string c) {
    bool returnbool = false;
    if (a[1] == b[1] && b[1] == c[1]) // check kind
      // check sequence or triple
      if ((int)a[0] - 47 == (int)b[0] - 48 &&
              (int)b[0] - 47 == (int)c[0] - 48 // sequence
          || a == b && b == c)                 // triple
        returnbool = !returnbool;
    return returnbool;
  }
public:
  string eye;
  void   CheckWin_SetTile(vector<string> tile) { this->tile = tile; };
  bool   CheckWin_Check(vector<string> hand, vector<vector<string>> meldhand,
                        vector<string> riverhand) {
    // declare
    bool           isgamingloop = true;
    bool           meld1;
    bool           meld2;
    vector<string> temphand;
    vector<string> eyes;
    // check playerhand is 8 and sort
    if (!meldhand.empty())
      for (vector<vector<string>>::iterator i = meldhand.begin();
           i != meldhand.end(); i++)
        if ((*i).size() == 4)
          hand.insert(hand.end(), 3, i->back());
        else
          hand.insert(hand.end(), i->begin(), i->end());
    if (hand.size() != 8)
      hand.push_back(riverhand.back());
    Sort(this->tile, hand);
    // clone playerhand to temphand
    this->hand = hand;
    // check there is a pair in palyerhand
    eyes = ProbablyEyes();
    if (eyes.size() == 4) {
      isgamingloop = false;
      this->eye = "4pair";
    } else {
      if (!eyes.empty()) { // eyes isn't empty
        for (vector<string>::iterator it = eyes.begin(); it != eyes.end();
             it++) {
          // check playerhand nee reset
          if (hand != this->hand)
            hand = this->hand;
          for (int i = 0; i < 2; i++)
            hand.erase(find(hand.begin(), hand.end(), *it));
          // [0][1][2] = meld1, [3][4][5] = meld2
          meld1 = CheckMeld(hand[0], hand[1], hand[2]);
          meld2 = CheckMeld(hand[3], hand[4], hand[5]);
          // meld1 is a meld and meld2 is a meld then set isgamingloop to false
          if (meld1 && meld2) {
            isgamingloop = false;
            this->eye = *it;
            break;
          }else{
						meld1 = CheckMeld(hand[0], hand[2], hand[4]);
						meld2 = CheckMeld(hand[1], hand[3], hand[5]);	
						if (meld1 && meld2){
							isgamingloop = false;
							this->eye = *it;
							break;
						}else
							isgamingloop = true;
					}
        }
      } else // eyes is empty
        isgamingloop = true;
    }
    return isgamingloop;
  };
  void GetEye(string &eye) { eye = this->eye; };
};

void DisCard(vector<string> &playerhand, vector<string> &riverhand) {
  char                     keyin;
  int                      target;
  vector<string>::iterator it;
  cout << "請選擇您要打出牌的編號(1-" << playerhand.size() << "):";
  cin >> keyin;
  while (isdigit(keyin) == false && (int)keyin != 1 && (int)keyin != 2 &&
         (int)keyin != 3 && (int)keyin != 4 && (int)keyin != 5) {
    cout << "請重新輸入:";
    cin >> keyin;
  }
  target = (int)keyin - 48; // set select number
  it = playerhand.begin() + target - 1;
  riverhand.push_back(playerhand[target - 1]);
  playerhand.erase(it);
}

// Kong class
class Kong {
	private:
		// DrawFromWallTail
		void DrawFromWallTail(vector<string> &hand, vector<string> &wall, bool &top){
			// check need to draw last or second last
			if(top){ // second last = top tile on pier
				hand.push_back(*(wall.rbegin() + 1));
				wall.erase(wall.end() - 2);
			}else{ // last = bottom on pier
				hand.push_back(wall.back());
				wall.erase(wall.end());
			}
			// switch between last and second last
			top = !top;
		};
	public:
		// big meld kong function
		void Big(vector<string> &hand, vector<vector<string>> &meldhand,
           vector<string> &river, vector<string> &wall, bool &top) {
			// remove the same as river last element from hand
			for(int i = 0; i < 3; i++)
				hand.erase(find(hand.begin(), hand.end(), river.back()));
			// append quadruple river last element to meldhand
			meldhand.push_back(
        {river.back(), river.back(), river.back(), river.back()});
			// remove river last element
			river.pop_back();
			DrawFromWallTail(hand, wall, top);
		};
		// Concealed and Small kong function
		void CheckConcealedAndSmall(vector<string> &hand, vector<vector<string>> &meldhand,
						 vector<string> &wall, bool &top, bool winonthewalltail){
			char key;
			// Concealed
			if (count(hand.begin(), hand.end(), hand.back()) == 4){
				cout << "要槓嗎(y/n)?";
				cin >> key;
				if (key == 'y') {
					meldhand.push_back({hand.back(), hand.back(), hand.back(), hand.back()});
					for (int i = 0; i < 3; i++)
						hand.erase(find(hand.begin(), hand.end(), hand.back()));
					hand.pop_back();
					DrawFromWallTail(hand, wall, top);
					winonthewalltail = !winonthewalltail;
				}
			// Small
			}else if (!meldhand.empty() && count(meldhand[0].begin(), meldhand[0].end(), hand.back()) == 3 ){
				cout << "要槓嗎(y/n)?";
        cin >> key;
				if (key == 'y') {
					meldhand[0].push_back(hand.back());
					hand.pop_back();
					DrawFromWallTail(hand, wall, top);
					winonthewalltail = !winonthewalltail;
				}
			};
		};
};

// check meld class inheritance from Kong class
class CheckMeld : public Kong {
private:
  // declare
  bool                   triple = false;
  bool                   quadruple = false;
  map<string, string>    tile;
  string                 target;
  vector<string>         hand;
  vector<string>         meldselect;
  vector<vector<string>> probablysequence;
  // check sequence function
  void CheckSequence() {
    string probably1 = to_string((int)this->target[0] - 50) + target[1];
    string probably2 = to_string((int)this->target[0] - 49) + target[1];
    string probably3 = to_string((int)this->target[0] - 47) + target[1];
    string probably4 = to_string((int)this->target[0] - 46) + target[1];
    vector<string>::iterator p1 =
        find(this->hand.begin(), this->hand.end(), probably1);
    vector<string>::iterator p2 =
        find(this->hand.begin(), this->hand.end(), probably2);
    vector<string>::iterator p3 =
        find(this->hand.begin(), this->hand.end(), probably3);
    vector<string>::iterator p4 =
        find(this->hand.begin(), this->hand.end(), probably4);
    if (p1 != this->hand.end() && p2 != this->hand.end())
      this->probablysequence.push_back({probably1, probably2});
    if (p2 != this->hand.end() && p3 != this->hand.end())
      this->probablysequence.push_back({probably2, probably3});
    if (p3 != this->hand.end() && p4 != this->hand.end())
      this->probablysequence.push_back({probably3, probably4});
  };
  // check triple and quadruple function
  void CheckTripleAndQuadruple() {
    if (count(this->hand.begin(), this->hand.end(), this->target) >= 2)
      this->triple = true;
    if (count(this->hand.begin(), this->hand.end(), this->target) == 3)
      this->quadruple = true;
  };
public:
  // declare
  char key;
  // Set Tile function
  void SetTile(map<string, string> tile) { this->tile = tile; };
  // Chekc function
  bool Check(vector<string> riverhand, vector<string> hand) {
    // set this->target = riverhand.back and this->hand = hand
    this->target = riverhand.back();
    this->hand = hand;
    // call CheckTripleAndQuadruple function
    CheckTripleAndQuadruple();
    // call CheckSequence function
    CheckSequence();
    if (!this->probablysequence.empty() || this->triple || this->quadruple)
      return true;
    else
      return false;
  };
  // player meld select function
  void MeldAskAndSelect() {
    // declare
    string maxrange = "";
  MeldChoice:
    // print question for meld yes or not
    // this->probablysequence isn't empty
    if (!this->probablysequence.empty()) {
      // and this->triple is true
      if (this->triple) {
        // and this->quadruple is true
        if (this->quadruple) {
          cout << "要吃(c)碰(p)槓(k)或跳過(s)嗎? ";
          maxrange = "cpks";
          // this->quadruple isn't true
        } else {
          cout << "要吃(c)碰(p)或跳過(s)嗎? ";
          maxrange = "cps";
        }
        // this->triple isn''t ture
      } else {
        cout << "要吃(c)或跳過(s)嗎? ";
        maxrange = "cs";
      }
      // player choice
      cin >> this->key;
      // check the enter is correct
      if (maxrange.empty()) {
        switch (maxrange.size()) {
        case 2:
          while (this->key != maxrange[0] && this->key != maxrange[1]) {
            cout << "請重新輸入選擇:";
            cin >> this->key;
            cout << endl;
          }
          break;
        case 3:
          while (this->key != maxrange[0] && this->key != maxrange[1] &&
                 this->key != maxrange[2]) {
            cout << "請重新輸入選擇:";
            cin >> this->key;
            cout << endl;
          }
          break;
        case 4:
          while (this->key != maxrange[0] && this->key != maxrange[1] &&
                 this->key != maxrange[2] && this->key != maxrange[3]) {
            cout << "請重新輸入選擇:";
            cin >> this->key;
            cout << endl;
          }
          break;
        }
      }
      // this-> probabllysequence is empty this->triple is true
    } else if (this->triple) {
      // and this->quadruple is true
      if (this->quadruple) {
        cout << "要碰(p)槓(k)或跳過(s)嗎? ";
        maxrange = "pks";
        // this->quadruple isn't ture
      } else {
        cout << "要碰(p)或跳過(s)嗎? ";
        maxrange = "ps";
      }
      // player choice
      cin >> this->key;
      // check the enter is correct
      if (maxrange.empty()) {
        switch (maxrange.size()) {
        case 2:
          while (this->key != maxrange[0] && this->key != maxrange[1]) {
            cout << "請重新輸入選擇:";
            cin >> this->key;
            cout << endl;
          }
          break;
        case 3:
          while (this->key != maxrange[0] && this->key != maxrange[1] &&
                 this->key != maxrange[2]) {
            cout << "請重新輸入選擇:";
            cin >> this->key;
            cout << endl;
          }
          break;
        }
      }
    } else {
      this->key = 's';
    }
  };
  void BeenMeld(vector<string> &hand, vector<vector<string>> &meldhand,
                vector<string> &river, vector<string> &wall, bool &top) {
    switch (this->key) {
    case 'c':
      Chi(hand, meldhand, river);
      break;
    case 'p':
      Pong(hand, meldhand, river);
      break;
    case 'k':
      Big(hand, meldhand, river, wall, top);
      break;
    case 's':
      DrawCard(wall, hand);
      break;
    }
  };
  // Chi function
  void Chi(vector<string> &hand, vector<vector<string>> &meldhand,
           vector<string> &river) {
    // declare
    vector<string> temphand;
    // check this->probablysequence is more than 1
    if (this->probablysequence.size() != 1) {
      // print this->probablysequence
      for (vector<vector<string>>::iterator probablyseq =
               this->probablysequence.begin();
           probablyseq != this->probablysequence.end();
           probablyseq++) {
        // print index
        cout << probablyseq - this->probablysequence.begin() + 1 << '.';
        // print probablysequence
        for (vector<string>::iterator ps = probablyseq->begin();
             ps != probablyseq->end(); ps++)
          cout << this->tile[*ps] << ' ';
        cout << ' ';
      }
      // show the warnning message and let player choice
      cout << endl
           << "有超過" << this->probablysequence.size() << "個選擇，"
           << "請選擇(1-" << this->probablysequence.size() << ")或是返回(r): ";
      // player enter choice
      cin >> this->key;
      // check the enter is correct
      switch (this->probablysequence.size()) {
      case 2:
        while (this->key != '1' && this->key != '2' && this->key != 'r') {
          cout << "請重新輸入選擇:";
          cin >> this->key;
        }
        break;
      case 3:
        while (this->key != '1' && this->key != '2' && this->key != '3' &&
               this->key != 'r') {
          cout << "請重新輸入選擇:";
          cin >> this->key;
        }
        break;
      }
      // set temphand = player choice
      switch (this->key) {
      case '1':
        temphand = this->probablysequence[0];
        break;
      case '2':
        temphand = this->probablysequence[1];
        break;
      case '3':
        temphand = this->probablysequence[2];
        break;
      }
    } else { // this->probablysequence is equal to 1
      temphand = this->probablysequence[0];
    }
    // make meld to meldhand from hand and riverhand
    hand.erase(find(hand.begin(), hand.end(), temphand[0]));
    hand.erase(find(hand.begin(), hand.end(), temphand[1]));
    // insert river last to temphand
    temphand.insert(temphand.begin() + 1, river.back());
    // remove river last element
    river.pop_back();
    // append temphand to meldhand
    meldhand.push_back(temphand);
  };
  // Pong function
  void Pong(vector<string> &hand, vector<vector<string>> &meldhand,
            vector<string> &river) {
    // remove the same as river last element from hand
    hand.erase(find(hand.begin(), hand.end(), river.back()));
    hand.erase(find(hand.begin(), hand.end(), river.back()));
    // append triple river last element to meldhand
    meldhand.push_back({river.back(), river.back(), river.back()});
    // remove river last element
    river.pop_back();
  }
  // reset check meld function
  void Reset() {
    this->target = "";
    this->hand.clear();
    this->meldselect.clear();
    this->probablysequence.clear();
    this->triple = false;
    this->quadruple = false;
    this->key = ' ';
  }
};

vector<string> TenPaiCheck(vector<string> playerhand, vector<string> tiles,
                           vector<vector<string>> playermeldhand) {
  CheckWin cw;
  cw.CheckWin_SetTile(tiles);
  vector<string>         probablywintile;
  vector<string>         checkhand;
  vector<string>         seventilehand;
  vector<string>         eighttilehand;
  vector<string>         emptyvector;
  vector<vector<string>> emptyvecvector;
  // set the tenpai hand
  if (!playermeldhand.empty())
    for (vector<vector<string>>::iterator meld = playermeldhand.begin();
         meld != playermeldhand.end(); meld++)
      playerhand.insert(playerhand.end(), meld->begin(), meld->end());
  // pop one by one from checkhand and append one by one from tile to check win
  for (vector<string>::iterator compareby = playerhand.begin();
       compareby != playerhand.end(); compareby++) {
    // reset checkhand(8tile) from playerhand(8tile)
    checkhand = playerhand;
    // try to pop one of checkhand for check win
    checkhand.erase(find(checkhand.begin(), checkhand.end(), *compareby));
    // clone checkhand(7tile) to seventilehand(7tile)
    seventilehand = checkhand;
    // try to append tile from tiles one by one to check win
    for (vector<string>::iterator tile = tiles.begin(); tile != tiles.end();
         tile++) {
      // reset checkhand(7tile) from seventile(7tile)
      checkhand = seventilehand;
      // append tile to checkhand(7tile) =checkhand(8tile)
      checkhand.push_back(*tile);
      // sort checkhand(8tile) for check win
      Sort(tiles, checkhand);
      // copy to eighttilehand from checkhand(8tile)
      eighttilehand = checkhand;
      // check the 4 paired
      if (!cw.CheckWin_Check(eighttilehand, emptyvecvector, emptyvector) &&
          cw.eye == "4pair") {
        // check the tile isn't in probablywintile
        if (find(probablywintile.begin(), probablywintile.end(), *tile) ==
            probablywintile.end()) {
          // then add to probablywintile
          probablywintile.push_back(*tile);
        }
        // check the tile is the probably win tile
      } else if (!cw.CheckWin_Check(eighttilehand, emptyvecvector,
                                    emptyvector)) {
        // check the tile isn't in probablywintile
        if (find(probablywintile.begin(), probablywintile.end(), *tile) ==
            probablywintile.end()) {
          // then add to probablywintile
          probablywintile.push_back(*tile);
        }
      }
    }
  }
  return probablywintile;
}

void DeclareTenPai(vector<string> probablywintile, bool &tenpai) {
  char key;
  if (!probablywintile.empty()) {
    cout << "要宣告聽牌嗎(y/n)? ";
    cin >> key;
    cout << endl;
    while (key != 'y' && key != 'n'){
      cout << "請重新輸入選擇: ";
			cin >> key;
			cout << endl;
		}
    tenpai = key == 'y' ? true : false;
  }
}

// AutoDiscard function
void AutoDisCard(vector<string> &playerhand, vector<string> &riverhand) {
  riverhand.push_back(playerhand.back());
  playerhand.pop_back();
}

// Score Class
class Score {
private:
  // delcare
  bool                   concealed = false;
  bool                   tsumo = false;
  bool                   lastwalltile = false;
  bool                   winonthewalltail = false;
  bool                   bookmaker = false;
  bool                   tenpai = false;
  int                    totaltai = 0;
  int                    gameround;
  int                    gameturn;
	int 									 continuetobookmaker = 0;
	map<string,string>     tile;
  string                 riverhandlastelement;
  string                 eye;
  vector<string>         sortby;
  vector<string>         playerhand;
  vector<vector<string>> playermeldhand;
  // 8 tai
  // tenhou function
  void TenHou() {
    if (this->gameround == 1 && this->gameturn == 0) {
      this->totaltai += 8;
      cout << "天胡        6台" << endl;
			Delay(750);
    }
  };
  // 4 tai
  // one dragon
  void OneDragon() {
    vector<string> temphand = this->playerhand;
    vector<string> compariso_b{"1b", "2b", "3b", "4b", "5b", "6b", "7b", "7b"};
    vector<string> compariso_r{"1r", "2r", "3r", "4r", "5r", "6r", "7r", "7r"};
    if (!this->playermeldhand.empty()) {
      for (vector<vector<string>>::iterator meld = this->playermeldhand.begin();
           meld != this->playermeldhand.end(); meld++)
        if ((*meld)[0] != (*meld)[1] && (*meld)[1] != (*meld)[2])
          temphand.insert(temphand.end(), meld->begin(), meld->end());
    }
    if (temphand.size() != 8)
      temphand.push_back(this->riverhandlastelement);
    Sort(this->sortby, temphand);
    if (temphand == compariso_b || temphand == compariso_r) {
      this->totaltai += 4;
      cout << "一條龍      4台" << endl;
			usleep(1000*750);
    }
  };
  // two dragon hug
  void TwoDragonHug() {
    vector<string> temphand;
    vector<string> compare1;
    vector<string> compare2;
    bool           turn = true;
    if (this->concealed) {
      temphand = this->playerhand;
      Sort(this->sortby, temphand);
      for (int i = 0; i < 2; i++)
        temphand.erase(find(temphand.begin(), temphand.end(), this->eye));
      for (vector<string>::iterator t = temphand.begin(); t != temphand.end();
           t++) {
        if (turn) {
          compare1.push_back(*t);
          turn = !turn;
        } else {
          compare2.push_back(*t);
          turn = !turn;
        }
      }
      if (compare1 == compare2) {
        this->totaltai += 4;
        cout << "雙龍抱      4台" << endl;
				usleep(1000*750);
      }
    }
  };
  // 3 or 1 tai
  // win on last draw tile or last discard tile
  void WinOnLastTile() {
    if (this->lastwalltile && this->tsumo) {
      this->totaltai += 3;
      cout << "海底撈月    3台" << endl;
			usleep(1000*750);
    } else if (this->lastwalltile && !this->tsumo) {
      this->totaltai += 1;
      cout << "河底撈魚    1台" << endl;
			usleep(1000*750);
    }
  };
  // concealed or not and tsumo or not
  void ConcealedOrNotAndTsumoOrNot() {
    if (this->concealed && this->tsumo) {
      this->totaltai += 3;
      cout << "門清自摸    3台" << endl;
			usleep(1000*750);
    } else if (this->concealed && !this->tsumo) {
      this->totaltai += 1;
      cout << "門清        1台" << endl;
			usleep(1000*750);
    } else if (!this->concealed && this->tsumo) {
      this->totaltai += 1;
      cout << "自摸        1台" << endl;
			usleep(1000*750);
    } else if (!this->concealed && !this->tsumo) {
      this->totaltai += 1;
      cout << "胡牌        1台" << endl;
			usleep(1000*750);
    }
  };
  // 2 tai
  // all paired
  void AllPaired() {
    vector<string> temphand;
    // copy playerhand to temphand
    temphand = this->playerhand;
    // if palyermeldhand isn't empty then add element to temphand
    if (!this->playermeldhand.empty()) {
      for (vector<vector<string>>::iterator meld = this->playermeldhand.begin();
           meld != this->playermeldhand.end(); meld++)
        if ((*meld)[0] == (*meld)[1] && (*meld)[1] == (*meld)[2])
          for (int i = 0; i < 3; i++)
            temphand.push_back((*meld)[0]);
    }
    // check the tempahnd isn't equal to 8
    if (temphand.size() != 8)
      temphand.push_back(this->riverhandlastelement);
    // sort temphand
    Sort(this->sortby, temphand);
    // pop out the eye from temphand
    for (int i = 0; i < 2; i++)
      temphand.erase(find(temphand.begin(), temphand.end(), this->eye));
    if (temphand[0] == temphand[1] && temphand[1] == temphand[2] &&
        temphand[3] == temphand[4] && temphand[4] == temphand[5]) {
      this->totaltai += 2;
      cout << "碰碰胡      2台" << endl;
			usleep(1000*750);
    }
  };
  // two kong
  void TwoKong() {
		if (this->playermeldhand.size() ==2)
    if (this->playermeldhand[0].size() == 4 &&
        this->playermeldhand[1].size() == 4) {
      this->totaltai += 2;
      cout << "二槓子      2台" << endl;
			usleep(1000*750);
    }
  };
  // win on the wall tail
  void WinOnTheWallTail() {
    if (this->winonthewalltail) {
      this->totaltai += 2;
      cout << "槓上開花    2台" << endl;
			usleep(1000*750);
    }
  };
  // 2 or 1 tai
  // all or half require
  void AllOrHalfRequire() {
    if (this->playermeldhand.size() == 2 && !this->tsumo) {
      this->totaltai += 2;
      cout << "全求        2台" << endl;
			usleep(1000*750);
    } else if (this->playermeldhand.size() == 2 && this->tsumo) {
      this->totaltai += 1;
      cout << "半求        1台" << endl;
			usleep(1000*750);
    }
  };
  // only or no general and sorider(chinlauto or danyau)
  void OnlyOrNoGeneralAndSorider() {
    vector<string> temphand;
    set<int>       compare;
    // copy playerhand to temphand
    temphand = this->playerhand;
    // if palyermeldhand isn't empty then add element to temphand
    if (!this->playermeldhand.empty()) {
      for (vector<vector<string>>::iterator meld = this->playermeldhand.begin();
           meld != this->playermeldhand.end(); meld++)
        temphand.insert(temphand.end(), meld->begin(), meld->end());
    }
    // check the tempahnd isn't equal to 8
    if (temphand.size() != 8)
      temphand.push_back(this->riverhandlastelement);
    // sort temphand
    Sort(this->sortby, temphand);
    // check only general and sorider
    for (vector<string>::iterator temp = temphand.begin();
         temp != temphand.end(); temp++) {
      // if there is 1 or 7 in temphand
      (*temp)[0] == '1' || (*temp)[0] == '7'
          ?
          // set false to true mean no generalandsorider
          compare.insert(1)
          : compare.insert(0);
    }
    // doesn't contain '1' and '7' but contain '2' ~ '6'
    if (find(compare.begin(), compare.end(), 1) == compare.end() &&
        find(compare.begin(), compare.end(), 0) != compare.end()) {
      this->totaltai += 1;
      cout << "段頭尾      1台" << endl;
			usleep(1000*750);
    } // contain '1' and '7' but not contain '2' ~ '6'
    else if (find(compare.begin(), compare.end(), 1) != compare.end() &&
             find(compare.begin(), compare.end(), 0) == compare.end()) {
      this->totaltai += 2;
      cout << "將帥領兵    2台" << endl;
			usleep(1000*750);
    }
  };
  // double tai
  // double
  void ContinueToBookmaker() {
    if (this->bookmaker) {
			if (this->continuetobookmaker > 0){
				this->totaltai += this->continuetobookmaker*2;
				cout << "連" << this->continuetobookmaker  << "拉" 
					<< this->continuetobookmaker  << "      " 
					<< this->continuetobookmaker * 2 << "台" << endl;  
				usleep(1000*750);
			}
		}
  };
  // 1 tai
  // same kind
  void SameKind() {
    bool           samekind = true;
    char           kind = ' ';
    vector<string> temphand;
    // copy playerhand to temphand
    temphand = this->playerhand;
    // if palyermeldhand isn't empty then add element to temphand
    if (!this->playermeldhand.empty()) {
      for (vector<vector<string>>::iterator meld = this->playermeldhand.begin();
           meld != this->playermeldhand.end(); meld++)
        if ((*meld)[0] != (*meld)[1] && (*meld)[1] != (*meld)[2])
          temphand.insert(temphand.end(), meld->begin(), meld->end());
    }
    // check the tempahnd isn't equal to 8
    if (temphand.size() != 8)
      temphand.push_back(this->riverhandlastelement);
    // sort temphand
    Sort(this->sortby, temphand);
    kind = temphand[0][1];
    for (vector<string>::iterator temp = temphand.begin();
         temp != temphand.end(); temp++) {
      if ((*temp)[1] != kind) {
        samekind = !samekind;
        break;
      }
    }
    if (samekind) {
      this->totaltai += 1;
      cout << "清一色      1台" << endl;
			usleep(1000*750);
    }
  };
  // four pair
  void FourPair() {
    vector<string> temphand;
    // copy playerhand to temphand
    temphand = this->playerhand;
    // if palyermeldhand isn't empty then add element to temphand
    if (!this->playermeldhand.empty()) {
      for (vector<vector<string>>::iterator meld = this->playermeldhand.begin();
           meld != this->playermeldhand.end(); meld++)
        if ((*meld)[0] != (*meld)[1] && (*meld)[1] != (*meld)[2])
          temphand.insert(temphand.end(), meld->begin(), meld->end());
    }
    // check the tempahnd isn't equal to 8
    if (temphand.size() != 8)
      temphand.push_back(this->riverhandlastelement);
    // sort temphand
    Sort(this->sortby, temphand);
    if (temphand[0] == temphand[1] && temphand[2] == temphand[3] &&
        temphand[4] == temphand[5] && temphand[6] == temphand[7]) {
      this->totaltai += 1;
      cout << "四對子      1台" << endl;
			usleep(1000*750);
    }
  };
  // bookmaker
  void BookMaker() {
    if (this->bookmaker) {
      this->totaltai += 1;
      cout << "莊家        1台" << endl;
			usleep(1000*750);
    }
  };
  // tenpai function
  void TenPai() {
    if (this->tenpai) {
      this->totaltai += 1;
      cout << "聽牌        1台" << endl;
			usleep(1000*750);
    }
  };
public:
	void SetTile(map<string,string> tile){
		this->tile = tile;
	};
  void SetAccordingTo(vector<string> sortby, vector<string> playerhand,
                      vector<vector<string>> playermeldhand,
                      vector<string> riverhand, string eye, int gameround,
                      int gameturn, bool concealed, bool tsumo,
                      bool lastwalltile, bool winonthewalltail, bool bookmaker,
                      int continuetobookmaker, bool tenpai) {
    this->sortby = sortby;
    this->playerhand = playerhand;
    this->playermeldhand = playermeldhand;
    this->riverhandlastelement = riverhand.back();
    this->gameround = gameround;
    this->gameturn = gameturn;
    this->concealed = concealed;
    this->eye = eye;
    this->tsumo = tsumo;
    this->lastwalltile = lastwalltile;
    this->winonthewalltail = winonthewalltail;
    this->bookmaker = bookmaker;
		this->continuetobookmaker = continuetobookmaker;
    this->tenpai = tenpai;
  };
  void PrintScore() {
		switch(this->gameturn){
			case 0:
				cout << "獲勝者是玩家1" << endl << endl; 
				usleep(1000*750);
				break;
			case 1:
				cout << "獲勝者是玩家2" << endl << endl; 
				break;
		}
		int ct = 0;
		ct += this->playerhand.size();
		for (vector<string>::iterator target = this->playerhand.begin();
				target != this->playerhand.end(); target ++)
			cout << this->tile[*target] << ' ';
		cout << "    ";
		if(!this->playermeldhand.empty())
		for(vector<vector<string>>::iterator target = this->playermeldhand.begin();
				target != this->playermeldhand.end(); target++){
			ct += target->size();
			for(vector<string>::iterator t = target->begin(); t != target->end(); t++)
				cout << this->tile[*t] << ' ';
		}
		cout << "    ";
		if(ct != 8){
			cout << this->tile[this->riverhandlastelement] ;
		}
		cout << endl << endl;
		TenHou();
    TenPai();
    BookMaker();
    ContinueToBookmaker();
    OnlyOrNoGeneralAndSorider();
    ConcealedOrNotAndTsumoOrNot();
    AllOrHalfRequire();
    WinOnLastTile();
    WinOnTheWallTail();
    FourPair();
    SameKind();
    TwoKong();
    AllPaired();
    TwoDragonHug();
    OneDragon();
		usleep(1000*750);
    cout << "===============" << endl;
		usleep(1000*750);
    cout << "共：        " << this->totaltai << "台" << endl;
  };
};

void ContinueToBookmakerOrChange(int &continuetobookmaker, int winner,
                                 bool &player1bookmaker, bool &player2bookmaker,
																 int &gameturn){
	switch(winner){
		case 1:
			if (player1bookmaker) // continue to bookmaker
				continuetobookmaker += 1;
			else{
				// change to next player too bookmaker
				gameturn = 0;
				// set player1 from player to bookmaker
				player1bookmaker = !player1bookmaker;
				// set player2 from bookmaker to player
				player2bookmaker = !player2bookmaker;
				// reset continue to bookmaker counter
				continuetobookmaker = 0;
			}
			break;
		case 2:
			if (player2bookmaker) // continue to bookmaker
				continuetobookmaker += 1;
			else{  
				// change to next player to bookmaker
				gameturn = 1; 
				// set player1 from bookmaker to player
				player1bookmaker = !player1bookmaker; 
				// set player2 from player to bookmaker
				player2bookmaker = !player2bookmaker;
				// reset continue to bookmaker counter
				continuetobookmaker = 0;
			}
			break;
		}
	}

int main() {
  // set random seed
  srand(unsigned(time(0)));
  // declare bool
  bool gameloop = false;
  bool tsumo = false;
  bool lastwalltile = false;
  bool winonthewalltail = false;
  bool player1tenpai = false;
  bool player2tenpai = false;
  bool player1concealed = false;
  bool player2concealed = false;
  bool player1bookmaker = true;
  bool player2bookmaker = false;
  bool ismeld = false;
  bool top = true;
  // declare int
  int gameturn = 0; // #0 = player1, #1 = player2
  int gameround = 1;
	int winner = 0;
	int continuetobookmaker = 0;
  // declare string
  string eye;
  // declare map
  map<string, string> tile = {{"1b", "黑將"}, {"2b", "黑士"}, {"3b", "黑象"},
                              {"4b", "黑車"}, {"5b", "黑馬"}, {"6b", "黑包"},
                              {"7b", "黑卒"}, {"1r", "紅帥"}, {"2r", "紅仕"},
                              {"3r", "紅相"}, {"4r", "紅俥"}, {"5r", "紅傌"},
                              {"6r", "紅炮"}, {"7r", "紅兵"}};
  // declare vector<string>
  vector<string> sortby{"1b", "2b", "3b", "4b", "5b", "6b", "7b",
                        "1r", "2r", "3r", "4r", "5r", "6r", "7r"};
  vector<string> wallhand;//{"1b","4b","5b","2b","1r"};
  vector<string> player1hand;//{"1b","1b","1b","2b","2b","2b","3b"};
  vector<string> player2hand;
  vector<string> riverhand;
  vector<string> player1probablywintile;
  vector<string> player2probablywintile;
  // declare vector<vector<string>>
  vector<vector<string>> player1meldhand;
  vector<vector<string>> player2meldhand;
  // declare class
  PrintAll  pa;
  CheckWin  cw;
  CheckMeld cm;
  Score     s;
  // pa class set name and map
  pa.PrintAll_SetNameAndMap("玩家1手牌", "玩家2手牌", "牌山", "牌河",
                            "玩家1組合手牌", "玩家2組合手牌", tile);
  // cw class set tile
  cw.CheckWin_SetTile(sortby);
  // cm class set tile
  cm.SetTile(tile);
	// s class set tile
	s.SetTile(tile);
ReStart:
	SetUp(player1bookmaker, player2bookmaker, tile, wallhand, player1hand, player2hand, sortby);
  // print round
  cout << "第 " << gameround << " 巡" << endl;
  // print all hand
  pa.PrintAll_Print(player1hand, player2hand, wallhand, riverhand,
                    player1meldhand, player2meldhand);
	switch(gameturn){
		case 0:
			// check player1 is tsumo(tenho)
			gameloop = cw.CheckWin_Check(player1hand, player1meldhand, riverhand);
			break;
		case 1:
			gameloop = cw.CheckWin_Check(player2hand, player2meldhand, riverhand);
			break;
	}
  if (!gameloop)
    cw.GetEye(eye);
Head:
  while (gameloop) {
    // change to next player
    gameturn != 1 ? gameturn += 1 : gameturn -= 1;
		// player2 discard and player1 check tsumo or ron
    if (gameturn == 0) {
			// check player2 is tenpai
      if (player2tenpai)
        AutoDisCard(player2hand, riverhand);
      else {
        // printplayer2hand with number index
        pa.PrintHand(2, player2hand, true);
				// check player2 is tenpai
        player2probablywintile =
            TenPaiCheck(player2hand, sortby, player2meldhand);
				// declare tenpai
        DeclareTenPai(player2probablywintile, player2tenpai);
        // discard from player2hand to riverhand
        DisCard(player2hand, riverhand);
      }
      // sort player2hand
      Sort(sortby, player2hand);
      // print all hand
      pa.PrintAll_Print(player1hand, player2hand, wallhand, riverhand,
                        player1meldhand, player2meldhand);
      // check player1 is ron
      gameloop = cw.CheckWin_Check(player1hand, player1meldhand, riverhand);
			// if player1 is ron 
      if (!gameloop){
        cw.GetEye(eye);
				// set gameturn to 0 mean turn to player1
        gameturn = 0;
			// if player1 isn't ron
			}else{ 
        // check player1 has meld with riverhand last
        ismeld = cm.Check(riverhand, player1hand);
				// if player1 is tenpai 
        if (player1tenpai){
					DrawCard(wallhand, player1hand);
					goto Player1TenpaiToAutoDrawCard;
				// player1 isn't tenpai
				}else{
					// player1 has meld
          if (ismeld) { 
            // ask player1 want meld or not
            cm.MeldAskAndSelect();
            // make meld from player1hand and riverhand to player1meldhand
            cm.BeenMeld(player1hand, player1meldhand, riverhand, wallhand, top);
						// if player1concealed = true and player1meldhand is empty 
						// set player1 isn't concealed player1concealed = false
            if (player1concealed && !player1meldhand.empty())
              player1concealed = !player1concealed;
            // key == s mean skip and draw card
            if (cm.key == 's') {
              goto Player1AfterDraw;
						// key == k mean kong and check is win on the wall tail
            }else if (cm.key == 'k'){
							// set winonthewalltail = true mean is konged
							winonthewalltail = !winonthewalltail;
							// check win after kong
							gameloop = cw.CheckWin_Check(player1hand, player1meldhand, 
								riverhand);
							// if player1 is winning
							if (!gameloop){
								// set eye 
								cw.GetEye(eye);
								// set tsumo to true
								tsumo = gameloop ? false : true;
								// go to gameloop to quite loop
								goto Head;
							// player1 isn't winning
							}else{
								// try to chek the kong again 
								cm.CheckConcealedAndSmall(player1hand, player1meldhand, wallhand, top, 
										winonthewalltail);
								// check win after check kong again
								gameloop = cw.CheckWin_Check(player1hand, player1meldhand, 
									riverhand);
								// if player1 is winning
								if (!gameloop){
									// set eye 
									cw.GetEye(eye);
									// set tsumo to true
									tsumo = gameloop ? false : true;
									// go to gameloop to quite loop
									goto Head;
								// palyer1 isn't winning
								}else{
									// set winonthewalltail to false mean isn't winning
									winonthewalltail = !winonthewalltail;
								}
							}
						}
					// player1 hasn't meld
          }else{ 
            // draw card
            DrawCard(wallhand, player1hand);
Player1AfterDraw:
            // check wall is empty mean flow
            if (wallhand.empty()) {
              gameloop = false;
						// wall isn't empty no flow
            } else {
Player1TenpaiToAutoDrawCard:
							// try to check concealed or small kong
							for(int i = 0; i < 2;i++){
								cm.CheckConcealedAndSmall(player1hand, player1meldhand, 
										wallhand, top, winonthewalltail);
								// if winthewalltail = true mean is konged
								if (winonthewalltail){
									// check player1 is winning by wall tail
									gameloop = cw.CheckWin_Check(player1hand, player1meldhand, 
											riverhand);
									// if player1 is winning
									if (!gameloop){
										// set eye 
										cw.GetEye(eye);
										// set tsumo to true
										tsumo = gameloop ? false : true;
										// go to gameloop to quite loop
										goto Head;
									// palyer1 isn't winning
									}else
										// set winonthewalltail to false mean isn't winning
										winonthewalltail = !winonthewalltail;
								}
							}
							// wall is empty mean flow
							if (wallhand.empty()) {
								gameloop = false;
							// wall isn't empty mean no flow
							}else{
								// check player1 is tsumo
								gameloop = cw.CheckWin_Check(player1hand, player1meldhand, 
										riverhand);
								// player1 isn't winning(tsumo)
								if (!gameloop) {
									// set eye
									cw.GetEye(eye);
									// set the tsumo bool to false or true
									tsumo = gameloop ? false : true;
									goto Head;
								}
							}
            }
					}
					// go to next round
					if (gameloop) {
						gameround += 1;
						// print round
						cout << "第 " << gameround << " 巡" << endl;
					}
					// reset check meld class
					cm.Reset();
					// print all hand
					pa.PrintAll_Print(player1hand, player2hand, wallhand, riverhand,
							player1meldhand, player2meldhand);
				}
			} 
		// player1 discard and player2 check tsumo or ron
    } else if (gameturn == 1) {
      // player1 is tenpai
      if (player1tenpai)
        AutoDisCard(player1hand, riverhand);
      else {
        // print player1hand with number index
        pa.PrintHand(1, player1hand, true);
        // check player1 is tenpai
        player1probablywintile =
            TenPaiCheck(player1hand, sortby, player1meldhand);
        // declare tenpai
        DeclareTenPai(player1probablywintile, player1tenpai);
        // discard from player1hand to riverhand
        DisCard(player1hand, riverhand);
      }
      // sort player1hand
      Sort(sortby, player1hand);
      // print all hand
      pa.PrintAll_Print(player1hand, player2hand, wallhand, riverhand,
                        player1meldhand, player2meldhand);
      // check player2 is ron
      gameloop = cw.CheckWin_Check(player2hand, player2meldhand, riverhand);
			// if player1 is ron
      if (!gameloop){
        cw.GetEye(eye);
        gameturn = 1;
			// if player2 isn't ron
			}else{ 
        // check player2 has meld with riverhand last
        ismeld = cm.Check(riverhand, player2hand);
        // if player2 is tenpai
        if (player2tenpai){
					DrawCard(wallhand, player2hand);
					goto Player2TenpaiToAutoDrawCard;
				// player2 isn't tenpai
				}else{
					// player2 has meld
          if (ismeld) { 
            // ask player2 wannt meld or not
            cm.MeldAskAndSelect();
            // make meld from player2hand and riverhand to player2meldhand
            cm.BeenMeld(player2hand, player2meldhand, riverhand, wallhand, top);
						// if player2concealed = true anad player2meldhand is empty
						// set player2 isn't concealed player2concealed = false
            if (player2concealed && !player2meldhand.empty())
              player2concealed = !player2concealed;
            // key == s mean skip and draw card
            if (cm.key == 's') {
              goto Player2AfterDraw;
						// key == k mean kong and check is win on the wall tail
            }else if (cm.key == 'k'){
							// set winonthewalltail = true mean is konged
							winonthewalltail = !winonthewalltail;
							// check win after kong
							gameloop = cw.CheckWin_Check(player2hand, player2meldhand,
									riverhand);
							// if player2 is winning
							if (!gameloop){
								// set eye
								cw.GetEye(eye);
								// set tsumo to true
								tsumo = gameloop ? false : true;
								// go to gameloop to quite loop
								goto Head;
							// player2 isn't winning
							}else{
								// try to chek kong again
								cm.CheckConcealedAndSmall(player1hand, player1meldhand, wallhand, top, 
										winonthewalltail);
								// check win after check kong again
								gameloop = cw.CheckWin_Check(player2hand, player2meldhand, 
										riverhand);
								// if player2 is winning
								if (!gameloop){
									// set eye
									cw.GetEye(eye);
									// set tsumo to true
									tsumo = gameloop ? false : true;
									// go to gameloop to quite loop
									goto Head;
								// player2 isn't winning
								}else
									// set winonthewalltail to false mean isn't winning
									winonthewalltail = !winonthewalltail;
							}
						}
					// player2 hasn't meld
          }else{ 
            // draw card
            DrawCard(wallhand, player2hand);
Player2AfterDraw:
            // check wall is empty mean flow
            if (wallhand.empty()) {
              gameloop = false;
						// wall isn't empty no flow
            } else {
Player2TenpaiToAutoDrawCard:
							// try to check concealed or small kong
							for(int i = 0; i < 2; i++){
								cm.CheckConcealedAndSmall(player1hand, player1meldhand, wallhand, top, 
										winonthewalltail);
								// if winthewalltai = true mean has konged
								if (winonthewalltail){
									// check player2 is winning
									gameloop = cw.CheckWin_Check(player2hand, player2meldhand, 
											riverhand);
									// if player2 is winning
									if (!gameloop){
										// set eye
										cw.GetEye(eye);
										// set tsumo to true
										tsumo = gameloop ? false : true;
										// go to gameloop to quite loop
										goto Head;
									// player2 isn't winning 
									}else
										// set winonthewalltail to false mean is't winning
										winonthewalltail = !winonthewalltail;
								}
							}
							// wall is empty mean flow
							if (wallhand.empty()) {
                gameloop = false;
							// wall isn't empty mean no flow
              } else {
                // check player2 is tsumo
                gameloop = cw.CheckWin_Check(player2hand, player2meldhand, 
										riverhand);
								// player2 isn't winning(tsumo)
                if (!gameloop) {
									// set eye
                  cw.GetEye(eye);
									// set the tsumo bool to false or true
                  tsumo = gameloop ? false : true;
									goto Head;
                }
              }
            }
          }
					// reset check meld class
					cm.Reset();
					// print all hand
					pa.PrintAll_Print(player1hand, player2hand, wallhand, riverhand,
														player1meldhand, player2meldhand);
        }
      }
    }
  }
   switch (gameturn) {
   case 0:
     s.SetAccordingTo(sortby, player1hand, player1meldhand, riverhand, eye,
                      gameround, gameturn, player1concealed, tsumo,
                      lastwalltile, winonthewalltail, player1bookmaker,
                      continuetobookmaker, player1tenpai);
		 winner = 1;
     break;
   case 1:
    s.SetAccordingTo(sortby, player2hand, player2meldhand, riverhand, eye,
                      gameround, gameturn, player2concealed, tsumo,
                      lastwalltile, winonthewalltail, player2bookmaker,
                      continuetobookmaker, player2tenpai);
	  winner = 2;
    break;
  }
	s.PrintScore();
	for(int i = 6; i > 0; i--){
		cout << i << "秒後開始下一輪" << endl; 
		sleep(1);
	}
	if (player2bookmaker && winner == 1) {
		return  0;
	}else{
		// reset value
		// bool
		gameloop = false;
		tsumo = false;
		lastwalltile = false;
		winonthewalltail = false;
		player1tenpai = false;
		player2tenpai = false;
		player1concealed = false;
		player2concealed = false;
		ismeld = false;
		top = true;
		// int
		gameround = 1;
		//  continuetobookmaker = 0;
		// string
		eye = "";
		// vector<string>
		wallhand.clear();
		player1hand.clear();
		player2hand.clear();
		riverhand.clear();
		player1probablywintile.clear();
		player2probablywintile.clear();
		// vector<vector<string>> 
		player1meldhand.clear();
		player2meldhand.clear();
		// change bookmaker or continue
		ContinueToBookmakerOrChange(continuetobookmaker, winner, player1bookmaker, 
				 player2bookmaker, gameturn);
		winner = 0;
		// class 
		pa = PrintAll();
		cw = CheckWin();
		cm = CheckMeld();
		s = Score();
		// class setname and tile
		pa.PrintAll_SetNameAndMap("玩家1手牌", "玩家2手牌", "牌山", "牌河",
															"玩家1組合手牌", "玩家2組合手牌", tile);
		// cw class set tile
		cw.CheckWin_SetTile(sortby);
		// cm class set tile
		cm.SetTile(tile);
		// s class set tile
		s.SetTile(tile);
		goto ReStart;
	}
}
