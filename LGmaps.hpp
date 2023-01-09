/* This is LGmaps.hpp file of LocalGen.                                  */
/* Copyright (c) 2023 LocalGen-dev; All rights reserved.                 */
/* Developers: http://github.com/LocalGen-dev                            */
/* Project: http://github.com/LocalGen-dev/LocalGen-new                  */
/*                                                                       */
/* This project is licensed under the MIT license. That means you can    */
/* download, use and share a copy of the product of this project. You    */
/* may modify the source code and make contribution to it too. But, you  */
/* must print the copyright information at the front of your product.    */
/*                                                                       */
/* The full MIT license this project uses can be found here:             */
/* http://github.com/LocalGen-dev/LocalGen-new/blob/main/LICENSE.md      */

#ifndef __LGMAPS_HPP__
#define __LGMAPS_HPP__

#include <string>
using std::string;
using std::to_string;

#define ll long long

struct Block {
	int team; /* the team who holds this block */
	int type; /* the block's type: 0->plain, 1->swamp, 2->mountain, 3->general, 4->city */
	ll army; /* count of army on this block */
};

int mapH,mapW;
Block gameMap[1005][1005]; /* maximum 1000*1000 */

struct teamS {
	string name; /* team name */
	int color; /* team color */
};
teamS defTeams[64] = {
	{"White",0xffffff},
	{"Red",0xff0000},
	{"Green",0x00ff00},
	{"Blue",0x0000ff},
	{"Yellow",0xffff00},
	{"Cyan",0x00ffff},
	{"Rose",0xff00ff},
	{"Orange",0xff8000},
	{"Lime",0x80ff00},
	{"Brown",0x804000},
	{"Grey",0x808080},
	{"Teal",0x008080},
	{"Purple",0xc000c0},
	{"Silver",0xc0c0c0},
	{"Maroon",0xc00000},
	{"Emerald",0x00ce80},
	{"Olive",0x808000},
};

struct playerCoord { int x,y; };

const char NUM_s[15]={0,'H','K','W','L','M','Q','Y','B','G','N','T'};
bool isVisible(int x,int y,int printCode) {
	for(int i=-1; i<=1; ++i) for(int j=-1; j<=1; ++j) if(printCode&(1<<gameMap[x+i][y+j].team)) return true;
	return false;
}
void printMap(int printCode,playerCoord coo) {
	setbcolor(0x000000);
	for(int i=1; i<=mapH; ++i) {
		for(int j=1; j<=mapW; ++j) {
			switch(gameMap[i][j].type) {
				case 0: /* plain */ {
					if(!gameMap[i][j].team) {
						setbcolor(0x000000);
						setfcolor(0xffffff);
						if(coo.x==i&&coo.y==j) setbcolor(0x000080);
						if(isVisible(i,j,printCode)) {
							if(!gameMap[i][j].army) fputs("     ",stdout);
							else if(gameMap[i][j].army<0) {
								int absd=-gameMap[i][j].army;
								if(absd<100) printf(" %3lld ",gameMap[i][j].army);
								else {
									string p=to_string(gameMap[i][j].army);
									printf(" %s%c ",p.substr(0,2).c_str(),NUM_s[p.size()-3]);
								}
							} else if(gameMap[i][j].army<1000) printf(" %3lld ",gameMap[i][j].army);
							else {
								string p=to_string(gameMap[i][j].army);
								printf(" %s%c ",p.substr(0,2).c_str(),NUM_s[p.size()-3]);
							}
						} else fputs("     ",stdout);
						setbcolor(0x000000);
					} else {
						setfcolor(defTeams[gameMap[i][j].team].color);
						if(coo.x==i&&coo.y==j) setbcolor(0x000080);
						if(isVisible(i,j,printCode)) {
							if(gameMap[i][j].army<0) {
								int absd=-gameMap[i][j].army;
								if(absd<100) printf(" %3lld ",gameMap[i][j].army);
								else {
									string p=to_string(gameMap[i][j].army);
									printf(" %s%c ",p.substr(0,2).c_str(),NUM_s[p.size()-3]);
								}
							} else if(gameMap[i][j].army<1000) printf(" %3lld ",gameMap[i][j].army);
							else {
								string p=to_string(gameMap[i][j].army);
								printf(" %s%c ",p.substr(0,2).c_str(),NUM_s[p.size()-3]);
							}
						} else fputs("     ",stdout);
						setbcolor(0x000000);
					}
					break;
				}
				case 1: /* swamp */ {
					if(!gameMap[i][j].team) {
						setbcolor(0x000000);
						setfcolor(0xffffff);
						if(coo.x==i&&coo.y==j) setbcolor(0x000080);
						fputs("=====",stdout);
						resetattr();
						setbcolor(0x000000);
					} else {
						setfcolor(defTeams[gameMap[i][j].team].color);
						if(coo.x==i&&coo.y==j) setbcolor(0x000080);
						if(isVisible(i,j,printCode)) {
							if(gameMap[i][j].army<10) printf("===%1lld=",gameMap[i][j].army);
							else if(gameMap[i][j].army<100) printf("==%2lld=",gameMap[i][j].army);
							else if(gameMap[i][j].army<1000) printf("=%3lld=",gameMap[i][j].army);
							else {
								string p=to_string(gameMap[i][j].army);
								printf("=%s%c=",p.substr(0,2).c_str(),NUM_s[p.size()-3]);
							}
						} else fputs("=====",stdout);
						setbcolor(0x000000);
					}
					break;
				}
				case 2: /* mountain */ {
					setbcolor(0x000000);
					setfcolor(0xffffff);
					if(coo.x==i&&coo.y==j) setbcolor(0x000080);
					fputs("#####",stdout);
					setbcolor(0x000000);
					break;
				}
				case 3: /* general */ {
					if(!gameMap[i][j].team) {
						setbcolor(0x000000);
						setfcolor(0xffffff);
						if(coo.x==i&&coo.y==j) setbcolor(0x000080);
						if(isVisible(i,j,printCode)) fputs("{GEN}",stdout);
						else fputs("     ",stdout);
						setbcolor(0x000000);
					}
					else {
						setfcolor(defTeams[gameMap[i][j].team].color);
						if(coo.x==i&&coo.y==j) setbcolor(0x000080);
						if(isVisible(i,j,printCode)) {
							if(gameMap[i][j].army<1000) printf("{%3lld}",gameMap[i][j].army);
							else {
								string p=to_string(gameMap[i][j].army);
								printf("{%s%c}",p.substr(0,2).c_str(),NUM_s[p.size()-3]);
							}
						} else fputs("     ",stdout);
						setbcolor(0x000000);
					}
					break;
				}
				case 4: /* city */ {
					if(!gameMap[i][j].team) {
						setbcolor(0x000000);
						setfcolor(0xffffff);
						if(coo.x==i&&coo.y==j) setbcolor(0x000080);
						if(isVisible(i,j,printCode)) {
							if(gameMap[i][j].army<0) {
								int absd=-gameMap[i][j].army;
								if(absd<100) printf("[%3lld]",gameMap[i][j].army);
								else {
									string p=to_string(gameMap[i][j].army);
									printf("[%s%c]",p.substr(0,2).c_str(),NUM_s[p.size()-3]);
								}
							} else if(gameMap[i][j].army<1000) printf("[%3lld]",gameMap[i][j].army);
							else {
								string p=to_string(gameMap[i][j].army);
								printf("[%s%c]",p.substr(0,2).c_str(),NUM_s[p.size()-3]);
							}
						} else fputs("#####",stdout);
						setbcolor(0x000000);
					} else {
						setfcolor(defTeams[gameMap[i][j].team].color);
						if(coo.x==i&&coo.y==j) setbcolor(0x000080);
						if(isVisible(i,j,printCode)) {
							if(gameMap[i][j].army<0) {
								int absd=-gameMap[i][j].army;
								if(absd<100) printf("[%3lld]",gameMap[i][j].army);
								else {
									string p=to_string(gameMap[i][j].army);
									printf("[%s%c]",p.substr(0,2).c_str(),NUM_s[p.size()-3]);
								}
							} else if(gameMap[i][j].army<1000) printf("[%3lld]",gameMap[i][j].army);
							else {
								string p=to_string(gameMap[i][j].army);
								printf("[%s%c]",p.substr(0,2).c_str(),NUM_s[p.size()-3]);
							}
						} else fputs("#####",stdout);
						setbcolor(0x000000);
					}
					break;
				}
			}
		}
		setfcolor(0x000000);
		clearline();
		putchar('|');
		putchar('\n');
	}
	fflush(stdout);
}

#undef ll // long long

#endif // __LGMAPS_HPP

