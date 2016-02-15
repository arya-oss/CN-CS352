#include <stdio.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <vector>
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

int IS_POWERof2 (int num) {
    return !(num & (num-1));
}

string hex2bin(string s) {
    string _tmp="";
    for (int i=0; i<s.length(); i++) {
        if(s[i] == '0') {
            _tmp += "0000";
        } else if(s[i] == '1') {
            _tmp += "0001";
        } else if(s[i] == '2') {
            _tmp += "0010";
        } else if(s[i] == '3') {
            _tmp += "0011";
        } else if(s[i] == '4') {
            _tmp += "0100";
        } else if(s[i] == '5') {
            _tmp += "0101";
        } else if(s[i] == '6') {
            _tmp += "0110";
        } else if(s[i] == '7') {
            _tmp += "0111";
        } else if(s[i] == '8') {
            _tmp += "1000";
        } else if(s[i] == '9') {
            _tmp += "1001";
        } else if(s[i] == 'A' || s[i] == 'a') {
            _tmp += "1010";
        } else if(s[i] == 'B' || s[i] == 'b') {
            _tmp += "1011";
        } else if(s[i] == 'C' || s[i] == 'c') {
            _tmp += "1100";
        } else if(s[i] == 'D' || s[i] == 'd') {
            _tmp += "1101";
        } else if(s[i] == 'E' || s[i] == 'e') {
            _tmp += "1110";
        } else if(s[i] == 'F' || s[i] == 'f') {
            _tmp += "1111";
        }
    }
    return _tmp;
}

int main() {
    string msg; ulli b_msg=0; short b, poly=0;
    cout << "Enter Message: ";
    cin >> msg;
    msg = hex2bin(msg);
    int msg_len = msg.length();
    for(int i=0; i < msg_len; i++) {
        b = msg[i]-'0';
        b_msg <<= 1;
        b_msg = BITSET(b_msg, 0, b);
    }
    // calculate r a/c to  2^r > n+r+1
    int r=0, _tmp=1;
    while(_tmp < msg_len+r+1) {
        _tmp <<= 1;
        r++;
    }

    vector<int> pos_bits[r]; vector<int>::iterator it;
    // get numbers which bit is 1 for each positon
    for (int i=0; i<4; i++) {
        for(int j=1; j<= msg_len+r; j++) {
            if(IS_BITSET(j,i))
                pos_bits[i].push_back(j);
        }
    }

    ulli _b_msg=0; int k=0;
    for (int i = 0; i < msg_len+r ; ++i) {
        if(!IS_POWERof2(i+1)) {
            if(IS_BITSET(b_msg, msg_len-k-1)){
                // cout<<i+1;
                _b_msg = BITSET(_b_msg, msg_len+r-i-1, 1);
            }
            k++;
        }
    }
    for (int i = 0; i < r; ++i) {
        int cnt=0;
        for(int j=0; j<pos_bits[i].size(); j++) {
            if(IS_BITSET(_b_msg, msg_len+r-pos_bits[i][j]))
                cnt++;
        }
        if (cnt&1) { // check cnt is odd for even parity
            _b_msg = BITSET(_b_msg, msg_len+r-(1<<i), 1);
        }
        // cout << cnt;
        // break;
    }
    cout << "Hamming Code: " << _b_msg << "\n";
    ulli save = _b_msg;
    string _hcode="";
    while(_b_msg != 0) {
        _hcode.insert(0, 1, (_b_msg%2)+48);
        _b_msg/=2;
    }
    if(_hcode.length() != msg_len+r) {
        _hcode.insert(0, msg_len+r-_hcode.length(), '0');
    }
    cout << "Hamming Code(binary: " << _hcode;
    cout << "Flipping Bit at 9th positon \n";
    save = BITSET(save, 3, 1);
    int p=0;
    for (int i = 0; i < r; ++i) {
        int cnt=0;
        for(int j=0; j<pos_bits[i].size(); j++) {
            if(IS_BITSET(save, msg_len+r-pos_bits[i][j]))
                cnt++;
        }
        if (cnt&1) { // check cnt is even for even parity
            p += 1<<i;
        }
        cout << cnt;
        // break;
    }
    cout <<"\nError at positon: "<< p << "\n";
    return 0;
}
