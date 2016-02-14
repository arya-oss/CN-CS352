#include <stdio.h>
#include <iostream>
#include <string>
#include <stdlib.h>
using namespace std;

#define ulli unsigned long long int

#define BITSET(dest, pos, val) (dest | (val<<pos))
#define IS_BITSET(val, pos) ((val) & (1<<(pos)))

int BIT_LENGTH(ulli num) {
    int cnt=0;
    while(num > 0) {
        num >>= 1;
        cnt++;
    }
}

int main() {
    string msg; ulli b_msg=0; short b, poly=0;
    cout << "Enter Message(binary): ";
    cin >> msg;
    int msg_len = msg.length();
    for(int i=0; i < msg_len; i++) {
        b = msg[i]-'0';
        b_msg <<= 1;
        b_msg = BITSET(b_msg, 0, b);
    }
    // x^3+x^2+1 => 1101
    cout << "Enter Polynomial Key(binary): ";
    string _poly;
    cin >> _poly;
    int degree = _poly.length()-1;
    for(int i=0; i <= degree; i++) {
        b = _poly[i]-'0';
        poly <<= 1;
        poly = BITSET(poly, 0, b);
    }
    // cout << poly;
    ulli _b_msg = b_msg;
    _b_msg <<= degree;
    short _tmp = _b_msg >> (msg_len);
    // cout << _b_msg << "\n";
    for(int i=1; i<=msg_len; i++) {
        _tmp <<= 1;
        if(IS_BITSET(_b_msg, msg_len-i)) {
            _tmp = _tmp|1;
        }
        // cout << _tmp << "\n";
        if(IS_BITSET(_tmp, degree)) {
            _tmp = _tmp^poly;
        } else {
            _tmp = _tmp^0;
        }
    }
    cout << "CRC(decimal): ";
    cout << _tmp << "\n";
    string _crc="";
    while(_tmp != 0) {
        _crc.insert(0, 1, (_tmp%2)+48);
        _tmp/=2;
    }
    if(_crc.length() != degree) {
        _crc.insert(0, degree-_crc.length(), '0');
    }
    cout << "CRC(binary): " << _crc <<"\n";
    cout << "Sent Message: " << msg+_crc << "\n";
    return 0;
}
