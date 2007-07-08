#ifndef _keyboard_h
#define _keyboard_h

#include <map>
#include <vector>

using namespace std;

/* handles allegro key[] array better than keypressed()
 * and readkey()
 */
class Keyboard{
public:

	Keyboard();

	/* poll:
	 * Put the keys in Allegro's key[] array into our map of int -> bool
	 */
	void poll();

	/* []:
	 * Extract a boolean value given a key number
	 */
	inline const bool operator[] ( const int i ){

		/* if the key has been pressed for the first time return true */
		if ( my_keys[ i ] < 0 ){
			my_keys[ i ] = 1;
			return true;
		}

		bool b = my_keys[ i ] > key_delay[ i ];
		if ( b ){
			my_keys[ i ] = 1;
		}
		return b;
	}

	/* keypressed:
	 * Returns true if a key is pressed
	 */
	const bool keypressed();
	
	/* readKeys:
	 * Store all pressed keys in a user supplied vector
	 */
	void readKeys( vector< int > & all_keys );

	void setDelay( const int key, const int delay );

	static const int Key_A;
	static const int Key_S;
	static const int Key_D;
	static const int Key_P;

	static const int Key_LEFT;
	static const int Key_RIGHT;
	static const int Key_UP;
	static const int Key_DOWN;
	static const int Key_SPACE;
	static const int Key_ENTER;

	static const int Key_F1;
	static const int Key_F2;
	static const int Key_F3;
	static const int Key_F4;
	static const int Key_F5;
	static const int Key_F6;
	static const int Key_F7;
	static const int Key_F8;
	static const int Key_F9;
	static const int Key_F10;
	static const int Key_F11;
	static const int Key_F12;
	static const int Key_ESC;
			
	static const int Key_MINUS_PAD;
	static const int Key_PLUS_PAD;
	static const int Key_ENTER_PAD;

protected:

	map<int,int> my_keys;
	map<int,int> key_delay;

};

#endif
