#include <iostream>
#include <stdio.h>
#include <string.h>
using namespace std;

int main() {
    char msg[32], poly[10]; int i, k=0, degree;
    cout << "Enter Message: ";
    cin >> msg;
    cout << "Enter Degree of Polynomial <= 10 : ";
    cin >> degree;
    for (i = 0; i <= degree; i++) {
        printf("degree of x^%d : ", degree-i);
        cin >> poly[i];
    }
    poly[i] = '\0';
    char crc[degree+2], msg_mod[20];
    memset(crc, '\0', sizeof(crc));
    memset(msg_mod, '\0', sizeof(msg_mod));
    strcpy(msg_mod, msg);
    memset(msg_mod+strlen(msg)-1, '0', degree);
    char poly_rev[10];
    strncpy(crc, msg_mod, degree+1);
    // cout << crc << " " << msg_mod;
    for (int i = 0; i < strlen(msg); i++) {
        // cout << "\n" << i << " ";
        if (crc[0] == poly[0]) {
            if (crc[0] == '1')
                strcpy(poly_rev, poly);
            else
                memset(poly_rev, '0', degree+1);
        } else {
            if (crc[0] == '0')
                memset(poly_rev, '0', degree+1);
            else
                strcpy(poly_rev, poly);
        }
        // cout << poly_rev << " " << crc;
        for (int j=1; j<=degree; j++) {
            if(crc[j] == poly_rev[j])
                crc[j-1] = '0';
            else
                crc[j-1] = '1';
        }
        crc[degree] = msg_mod[degree+1+i];
    }
    cout << "CRC: " << crc << "\n";
    return 0;
}
