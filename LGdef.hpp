#ifndef __LGDEF_HPP__
#define __LGDEF_HPP__

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <chrono>
#include <algorithm>
#include <graphics.h>
#include "graphics/LGGrectbut.hpp"

using std::string;
using std::to_string;

const int LEN_ZIP=200005;
const char NUM_s[20] = {0, 'H', 'K', 'W', 'L', 'M', 'Q', 'I', 'G', 'B', 'N', 'T'};

PIMAGE pimg[7];

struct MapInfoS {
	int id;
	string chiname;
	string engname;
	string auth;
	int hei;
	int wid;
	int generalcnt;
	int swampcnt;
	int citycnt;
	int mountaincnt;
	int plaincnt;
	string filename;
	MapInfoS() = default;
	~MapInfoS() = default;
};MapInfoS maps[5005];

struct Block {
	int team; /* the team who holds this block */
	int type; /* the block's type: 0->plain, 1->swamp, 2->mountain, 3->general, 4->city */
	long long army;  /* count of army on this block */
	bool lit; /* whether the block is lighted(lit) */
};

int mapH, mapW;
int widthPerBlock, heightPerBlock;
Block gameMap[505][505]; /* maximum 500*500 */

struct teamS {
	string name;		/* team name */
	unsigned int color; /* team color */
};teamS defTeams[64] = {
	{"White", 0xffffffff},
	{"Red", 0xffff0000},
	{"Aqua", 0xff4363d8},
	{"Green", 0xff008000},
	{"Teal", 0xff008080},
	{"Orange", 0xfff58231},
	{"Pink", 0xfff032e6},
	{"Purple", 0xff800080},
	{"Maroon", 0xff800000},
	{"Yellow", 0xffb09f30},
	{"Brown", 0xff9a6324},
	{"Blue", 0xff0000ff},
	{"Indigo", 0xff483d8b},
};

struct playerCoord {
	int x, y;
};bool operator== (playerCoord a,playerCoord b) {
	return a.x==b.x&&a.y==b.y;
}

bool isVisible(int,int,int);
void printNum(bool visible, long long army, int team, int curx, int cury);

void ege_circle(int x, int y, int r) {
	ege_ellipse(x - r, y - r, r << 1, r << 1);
}

bool FullScreen(HWND hwnd, int fullscreenWidth = GetSystemMetrics(SM_CXSCREEN), int fullscreenHeight = GetSystemMetrics(SM_CYSCREEN), int colourBits = 32, int refreshRate = 60) {
	DEVMODE fullscreenSettings;
	bool isChangeSuccessful;
	RECT windowBoundary;

	EnumDisplaySettings(NULL, 0, &fullscreenSettings);
	fullscreenSettings.dmPelsWidth = fullscreenWidth;
	fullscreenSettings.dmPelsHeight = fullscreenHeight;
	fullscreenSettings.dmBitsPerPel = colourBits;
	fullscreenSettings.dmDisplayFrequency = refreshRate;
	fullscreenSettings.dmFields = DM_PELSWIDTH |
	                              DM_PELSHEIGHT |
	                              DM_BITSPERPEL |
	                              DM_DISPLAYFREQUENCY;

	SetWindowLongPtr(hwnd, GWL_EXSTYLE, WS_EX_APPWINDOW | WS_EX_TOPMOST);
	SetWindowLongPtr(hwnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);
	SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, fullscreenWidth, fullscreenHeight, SWP_SHOWWINDOW);
	isChangeSuccessful = ChangeDisplaySettings(&fullscreenSettings, CDS_FULLSCREEN) == DISP_CHANGE_SUCCESSFUL;
	ShowWindow(hwnd, SW_MAXIMIZE);

	return isChangeSuccessful;
}

namespace imageOperation {
	void zoomImage(PIMAGE& pimg, int zoomWidth, int zoomHeight) {
		if((pimg == NULL) || (zoomWidth == getwidth(pimg) && zoomHeight == getheight(pimg)))
			return;

		PIMAGE zoomImage = newimage(zoomWidth, zoomHeight);
		putimage(zoomImage, 0, 0, zoomWidth, zoomHeight, pimg, 0, 0, getwidth(pimg), getheight(pimg));
		delimage(pimg);

		pimg = zoomImage;
	}
	void setWindowTransparent(bool enable, int alpha = 0xFF) {
		HWND egeHwnd = getHWnd();
		LONG nRet = ::GetWindowLong(egeHwnd, GWL_EXSTYLE);
		nRet |= WS_EX_LAYERED;
		::SetWindowLong(egeHwnd, GWL_EXSTYLE, nRet);
		if(!enable)
			alpha = 0xFF;
		SetLayeredWindowAttributes(egeHwnd, 0, alpha, LWA_ALPHA);
	}
}

namespace LGGraphics {
	string fileName;
	int stDel = 1;
	int plCnt = 0;
	int mapSelected = 0;
	int cheatCode = 0;
	struct mapData {
		int heightPerBlock;
		int widthPerBlock;
		int height, width;
		double mapSizeX, mapSizeY;
	} mapDataStore;
	void selectOrImportMap();
	void doMapImport();
	void WelcomePage();
	void doMapSelect();
	void importGameSettings();
	void inputMapData(int a, int b, int c, int d) {
		mapDataStore.heightPerBlock = a;
		mapDataStore.widthPerBlock = b;
		mapDataStore.height = c;
		mapDataStore.width = d;
		return;
	}
	int select = 0;
	void initWindowSize() {
		initgraph(800, 600);
		ege_enable_aa(true, NULL);
		PIMAGE favi = newimage();
		getimage_pngfile(favi, "img/favicon.png");
		setcaption("LocalGen v" VER_STRING " Window Size Selection");
		setbkcolor(WHITE);
		setbkcolor_f(WHITE);
		bool changeMade = true;
		cleardevice();
		settextjustify(CENTER_TEXT, CENTER_TEXT);
		setfont(200, 0, "Freestyle Script");
		setcolor(BLUE);
		xyprintf(250, 125, "LocalGen");
		putimage_withalpha(NULL, favi, 500, 10, 0, 0, getwidth(favi), getheight(favi));
		delimage(favi);
		setfont(20, 0, "Lucida Fax");
		xyprintf(500 + getwidth(favi) / 2, 10 + getheight(favi) + 20 / 2 + 10,
		         ("version " + to_string(VER_MAJOR) + "." + to_string(VER_MINOR)
		          + "." + to_string(VER_RELEASE) + " (build " + to_string(VER_BUILD) + ")").c_str());
		setfont(50, 0, "Freestyle Script");
		xyprintf(250, 250, "Please Select Window Size:");
		settextjustify(LEFT_TEXT, TOP_TEXT);
		rectBUTTON scrsz[10];
		for(int i = 200; i <= 500; i += 100) {
			register int p = i / 100 - 2;
			scrsz[p].sethw(50, 400);
			scrsz[p].setbgcol(0xffffffff);
			scrsz[p].settxtcol(BLUE);
			scrsz[p].setfontname("Freestyle Script");
			scrsz[p].setfonth(40);
			scrsz[p].setlocation(180 + i / 4 * 2, 50);
			scrsz[p].addtext(to_string(i * 4) + " �� " + to_string(i * 9 / 4));
			scrsz[p].clickEvent = [i]()->void { select = i / 100; };
			scrsz[p].setalign(CENTER_TEXT, CENTER_TEXT);
			scrsz[p].display();
		} {
			int i = 600;
			register int p = i / 100 - 2;
			scrsz[p].sethw(50, 400);
			scrsz[p].setbgcol(0xffffffff);
			scrsz[p].settxtcol(BLUE);
			scrsz[p].setfontname("Freestyle Script");
			scrsz[p].setfonth(40);
			scrsz[p].setlocation(180 + i / 4 * 2, 50);
			scrsz[p].addtext("Auto Fit (Full Screen)");
			scrsz[p].clickEvent = [i]()->void { select = i / 100; };
			scrsz[p].setalign(CENTER_TEXT, CENTER_TEXT);
			scrsz[p].display();
		}
		for(; is_run(); delay_fps(60)) {
			for(int i = 200; i <= 600; i += 100) {
				register int p = i / 100 - 2;
				register int k = scrsz[p].status;
				scrsz[p].detect();
				scrsz[p].display();
				if(scrsz[p].status == 2) scrsz[p].clickEvent(), changeMade = false;
			}
			if(!changeMade) break;
		}
	finishSelect:
		setcaption("LocalGen v" VER_STRING " developed by LocalGen-dev");
		if(select == 6) {
//			movewindow(0, 0, false);
			resizewindow(-1, -1);
			FullScreen(getHWnd());
			int w = getmaxx(), h = getmaxy();
			mapDataStore.mapSizeX = (double)(1.0 * (double)w / 1600.0);
			mapDataStore.mapSizeY = (double)(1.0 * (double)h / 900.0);
		} else {
			mapDataStore.mapSizeX = mapDataStore.mapSizeY = (double)select / 4.0;
			int nScreenWidth, nScreenHeight;
			nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
			nScreenHeight = GetSystemMetrics(SM_CYSCREEN);
			initgraph(1600 * mapDataStore.mapSizeX, 900 * mapDataStore.mapSizeY, RENDER_AUTO);
//			movewindow((nScreenWidth - 1600 * mapDataStore.mapSize) / 2, (nScreenHeight - 900 * mapDataStore.mapSize) / 2, true);
		}
	}

	void WelcomePage() {
		initWindowSize();
		setbkmode(TRANSPARENT);
		setbkcolor(WHITE);
		setbkcolor_f(WHITE);
		cleardevice();
		// xyprintf(100, 100, "%f", mapDataStore.mapSize);
		settextjustify(CENTER_TEXT, TOP_TEXT);
		setfont(500 * mapDataStore.mapSizeY, 0, "Freestyle Script");
		setcolor(BLUE);
		xyprintf(800 * mapDataStore.mapSizeY, 0, "LocalGen");
		rectBUTTON singbut;
		singbut.setbgcol(GREEN);
		singbut.sethw(200 * mapDataStore.mapSizeY, 400 * mapDataStore.mapSizeX);
		singbut.setlocation(600 * mapDataStore.mapSizeY, 300 * mapDataStore.mapSizeX);
		singbut.settxtcol(WHITE);
		singbut.setfontname("Freestyle Script");
		singbut.setfonthw(100 * mapDataStore.mapSizeY, 0);
		singbut.setalign(CENTER_TEXT, CENTER_TEXT);
		singbut.addtext("Single Player");
		singbut.setlnwid(10 * mapDataStore.mapSizeY);
		singbut.display();
		rectBUTTON multibut;
		multibut.setbgcol(RED);
		multibut.sethw(200 * mapDataStore.mapSizeY, 400 * mapDataStore.mapSizeX);
		multibut.setlocation(600 * mapDataStore.mapSizeY, 900 * mapDataStore.mapSizeX);
		multibut.settxtcol(WHITE);
		multibut.setfontname("Freestyle Script");
		multibut.setfonthw(100 * mapDataStore.mapSizeY, 0);
		multibut.setalign(CENTER_TEXT, CENTER_TEXT);
		multibut.addtext("Multiplayer");
		multibut.setlnwid(10 * mapDataStore.mapSizeY);
		multibut.display();
		delay_ms(0);
		for(; is_run(); delay_fps(120)) {
			singbut.detect();
			singbut.display();
			if(singbut.status == 2) break;
			multibut.detect();
			multibut.display();
			if(multibut.status == 2) {
				settextjustify(CENTER_TEXT, CENTER_TEXT);
				setfont(100 * mapDataStore.mapSizeY, 0, "Freestyle Script");
				xyprintf(800 * mapDataStore.mapSizeX, 500 * mapDataStore.mapSizeY, "Sorry! Multiplayer Mode is still developing.");
				Sleep(4000);
				exitExe();
			}
		}
		settextjustify(LEFT_TEXT, TOP_TEXT);
	}

	void selectOrImportMap() {
		setbkmode(TRANSPARENT);
		rectBUTTON selbut;
		selbut.setbgcol(BROWN);
		selbut.setlocation(600 * mapDataStore.mapSizeY, 300 * mapDataStore.mapSizeX);
		selbut.sethw(200 * mapDataStore.mapSizeY, 400 * mapDataStore.mapSizeX);
		selbut.settxtcol(WHITE);
		selbut.setfontname("Freestyle Script");
		selbut.setfonthw(100 * mapDataStore.mapSizeY, 0);
		selbut.addtext("Choose a Map");
		selbut.setalign(CENTER_TEXT, CENTER_TEXT);
		selbut.setlnwid(10 * mapDataStore.mapSizeY);
		rectBUTTON impbut;
		impbut.setbgcol(BROWN);
		impbut.setlocation(600 * mapDataStore.mapSizeY, 900 * mapDataStore.mapSizeX);
		impbut.sethw(200 * mapDataStore.mapSizeY, 400 * mapDataStore.mapSizeX);
		impbut.settxtcol(WHITE);
		impbut.setfontname("Freestyle Script");
		impbut.setfonthw(100 * mapDataStore.mapSizeY, 0);
		impbut.addtext("Import a Map");
		impbut.setalign(CENTER_TEXT, CENTER_TEXT);
		impbut.setlnwid(10 * mapDataStore.mapSizeY);
		int select = -1;
		while(select == -1) {
			selbut.detect();
			selbut.display();
			if(selbut.status == 2) select = true;
			impbut.detect();
			impbut.display();
			if(impbut.status == 2) select = false;
		}
		cleardevice();
		if(!select) doMapImport();
		else doMapSelect();
		importGameSettings();
	}

	void doMapSelect() {
//		int mapNum = returnMapNum();
		cleardevice();
//		if (!isdllOK()) {
//			xyprintf(10 * mapDataStore.mapSizeX, 10 * mapDataStore.mapSizeY, "Oops, it seems that your 'defMap.dll' is missing!\n You can download it on GitHub. ");
//			Sleep(4000);
//			exitExe();
//		}
		int left, right, up, down;
		int x, y;
		rectBUTTON mapbut[50];
		for(int i = 1; i <= mapNum; i++) {
			x = (i + 5) / 6;
			y = ((i % 6 == 0) ? 6 : i % 6);
			left = ((y - 1) * 260) * mapDataStore.mapSizeX;
			right = (y * 260) * mapDataStore.mapSizeX;
			up = ((x - 1) * 180) * mapDataStore.mapSizeY;
			down = (x * 180) * mapDataStore.mapSizeY;
			mapbut[i].sethw(down - up, right - left);
			mapbut[i].setlocation(up, left);
			mapbut[i].setfontname("Segoe UI");
			mapbut[i].setfonthw(20 * mapDataStore.mapSizeY, 0);
			mapbut[i].settxtcol(BLUE);
			mapbut[i].setbgcol(WHITE);
			mapbut[i].addtext("id: " + to_string(maps[i].id) + " " + maps[i].chiname);
			mapbut[i].addtext(maps[i].engname);
			mapbut[i].addtext("General Count: " + to_string(maps[i].generalcnt));
			mapbut[i].addtext("Plain Count: " + to_string(maps[i].plaincnt));
			mapbut[i].addtext("City Count: " + to_string(maps[i].citycnt));
			mapbut[i].addtext("Mountain Count: " + to_string(maps[i].mountaincnt));
			mapbut[i].addtext("Swamp Count: " + to_string(maps[i].swampcnt));
			mapbut[i].addtext("Size: " + to_string(maps[i].hei) + " * " + to_string(maps[i].wid));
			mapbut[i].setalign(CENTER_TEXT, CENTER_TEXT);
			mapbut[i].clickEvent = [i]()->void { mapSelected = i; };
			// imageOperation::zoomImage(pimg[5], 200, 200);
			// putimage_transparent(NULL, pimg[5], left, up, getpixel(0, 0, pimg[1]));
//			setcolor(BLUE);
//			setfont(18 * mapDataStore.mapSize, 0, "Segoe UI");
//			rectprintf(left, (up + 1 * mapDataStore.mapSize), 135 * mapDataStore.mapSize, 40 * mapDataStore.mapSize, "id:%02d %s", maps[i].id, maps[i].chiname.c_str());
//			setfont(16 * mapDataStore.mapSize, 0, "Segoe UI");
//			rectprintf(left, (up + 40 * mapDataStore.mapSize), 162 * mapDataStore.mapSize, 18 * mapDataStore.mapSize, "%s", maps[i].engname.c_str());
//			rectprintf(left, (up + 60 * mapDataStore.mapSize), 162 * mapDataStore.mapSize, 18 * mapDataStore.mapSize, "GeneralCount: %d", maps[i].generalcnt);
//			rectprintf(left, (up + 80 * mapDataStore.mapSize), 162 * mapDataStore.mapSize, 18 * mapDataStore.mapSize, "SwampCount: %d", maps[i].swampcnt);
//			rectprintf(left, (up + 100 * mapDataStore.mapSize), 162 * mapDataStore.mapSize, 18 * mapDataStore.mapSize, "MountainCount: %d", maps[i].mountaincnt);
//			rectprintf(left, (up + 120 * mapDataStore.mapSize), 162 * mapDataStore.mapSize, 18 * mapDataStore.mapSize, "CityCount: %d", maps[i].citycnt);
//			rectprintf(left, (up + 140 * mapDataStore.mapSize), 162 * mapDataStore.mapSize, 18 * mapDataStore.mapSize, "Size: %d * %d", maps[i].hei, maps[i].wid);
		}
		mouse_msg msg;
		for(; is_run(); delay_fps(60)) {
			for(int i = 1; i <= mapNum; ++i) {
				mapbut[i].detect();
				mapbut[i].display();
				if(mapbut[i].status == 2) mapbut[i].clickEvent();
			}
			if(mapSelected) break;
		}
		cleardevice();
		setcolor(GREEN);
		setfont(40 * mapDataStore.mapSizeY, 0, "Lucida Fax");
		xyprintf(10 * mapDataStore.mapSizeX, 10 * mapDataStore.mapSizeY, "id: %02d", maps[mapSelected].id);
		xyprintf(10 * mapDataStore.mapSizeX, 40 * mapDataStore.mapSizeY, "%s", maps[mapSelected].chiname.c_str());
		setfont(30 * mapDataStore.mapSizeY, 0, "Lucida Fax");
		xyprintf(300 * mapDataStore.mapSizeX, 40 * mapDataStore.mapSizeY, "%s", maps[mapSelected].engname.c_str());
		xyprintf(10 * mapDataStore.mapSizeX, 70 * mapDataStore.mapSizeY, "Author: %s", maps[mapSelected].auth.c_str());
		xyprintf(10 * mapDataStore.mapSizeX, 100 * mapDataStore.mapSizeY, "Size of the Map: %d * %d", maps[mapSelected].hei, maps[mapSelected].wid);
		xyprintf(10 * mapDataStore.mapSizeX, 130 * mapDataStore.mapSizeY, "GeneralCount : %d          PlainCount: %d", maps[mapSelected].generalcnt, maps[mapSelected].plaincnt);
		xyprintf(10 * mapDataStore.mapSizeX, 160 * mapDataStore.mapSizeY, "MountainCount: %d          CityCount : %d", maps[mapSelected].mountaincnt, maps[mapSelected].citycnt);
		if(mapSelected < 6) {
			setfont(30 * mapDataStore.mapSizeY, 0, "Lucida Fax");
			setcolor(MAGENTA);
			int height = 0;
			key_msg msg;
			xyprintf(10 * mapDataStore.mapSizeX, 200 * mapDataStore.mapSizeY, "Please Input the Height of the Map (<=500)");
			while(1) {
				msg = getkey();
				if(msg.msg == key_msg_char) {
					if(msg.key == 13 && height <= 500 && height >= 1)
						break;
					if(msg.key == 8)
						height /= 10;
					else if(msg.key >= '0' && msg.key <= '9')
						height = height * 10 + msg.key - '0';
					cleardevice();
					setcolor(GREEN);
					setfont(40 * mapDataStore.mapSizeY, 0, "Lucida Fax");
					xyprintf(10 * mapDataStore.mapSizeX, 10 * mapDataStore.mapSizeY, "id: %02d", maps[mapSelected].id);
					xyprintf(10 * mapDataStore.mapSizeX, 40 * mapDataStore.mapSizeY, "%s", maps[mapSelected].chiname.c_str());
					setfont(30 * mapDataStore.mapSizeY, 0, "Lucida Fax");
					xyprintf(300 * mapDataStore.mapSizeX, 40 * mapDataStore.mapSizeY, "%s", maps[mapSelected].engname.c_str());
					xyprintf(10 * mapDataStore.mapSizeX, 70 * mapDataStore.mapSizeY, "Author of the Map: %s", maps[mapSelected].auth.c_str());
					xyprintf(10 * mapDataStore.mapSizeX, 100 * mapDataStore.mapSizeY, "Size of the Map: %d * %d", maps[mapSelected].hei, maps[mapSelected].wid);
					xyprintf(10 * mapDataStore.mapSizeX, 130 * mapDataStore.mapSizeY, "GeneralCount : %d          PlainCount: %d", maps[mapSelected].generalcnt, maps[mapSelected].plaincnt);
					xyprintf(10 * mapDataStore.mapSizeX, 160 * mapDataStore.mapSizeY, "MountainCount: %d          CityCount : %d", maps[mapSelected].mountaincnt, maps[mapSelected].citycnt);
					setcolor(MAGENTA);
					xyprintf(10 * mapDataStore.mapSizeX, 200 * mapDataStore.mapSizeY, "Please Input the Height of the Map (<=500)");
					xyprintf(10 * mapDataStore.mapSizeX, 230 * mapDataStore.mapSizeY, "%d", height);
				}
			}
			cleardevice();
			setcolor(GREEN);
			setfont(40 * mapDataStore.mapSizeY, 0, "Lucida Fax");
			xyprintf(10 * mapDataStore.mapSizeX, 10 * mapDataStore.mapSizeY, "id: %02d", maps[mapSelected].id);
			xyprintf(10 * mapDataStore.mapSizeX, 40 * mapDataStore.mapSizeY, "%s", maps[mapSelected].chiname.c_str());
			setfont(30 * mapDataStore.mapSizeY, 0, "Lucida Fax");
			xyprintf(300 * mapDataStore.mapSizeX, 40 * mapDataStore.mapSizeY, "%s", maps[mapSelected].engname.c_str());
			xyprintf(10 * mapDataStore.mapSizeX, 70 * mapDataStore.mapSizeY, "Author of the Map: %s", maps[mapSelected].auth.c_str());
			xyprintf(10 * mapDataStore.mapSizeX, 100 * mapDataStore.mapSizeY, "Size of the Map: %d * %d", maps[mapSelected].hei, maps[mapSelected].wid);
			xyprintf(10 * mapDataStore.mapSizeX, 130 * mapDataStore.mapSizeY, "GeneralCount : %d          PlainCount: %d", maps[mapSelected].generalcnt, maps[mapSelected].plaincnt);
			xyprintf(10 * mapDataStore.mapSizeX, 160 * mapDataStore.mapSizeY, "MountainCount: %d          CityCount : %d", maps[mapSelected].mountaincnt, maps[mapSelected].citycnt);
			setcolor(MAGENTA);
			xyprintf(10 * mapDataStore.mapSizeX, 200 * mapDataStore.mapSizeY, "Please Input the Height of the Map (<=500)");
			xyprintf(10 * mapDataStore.mapSizeX, 230 * mapDataStore.mapSizeY, "%d", height);
			int width = 0;
			xyprintf(810 * mapDataStore.mapSizeX, 200 * mapDataStore.mapSizeY, "Please Input the Width of the Map (<=500)");
			while(1) {
				msg = getkey();
				if(msg.msg == key_msg_char) {
					if(msg.key == 13 && width <= 500 && width >= 1)
						break;
					if(msg.key == 8)
						width /= 10;
					else if(msg.key >= '0' && msg.key <= '9')
						width = width * 10 + msg.key - '0';
					cleardevice();
					setcolor(GREEN);
					setfont(40 * mapDataStore.mapSizeY, 0, "Lucida Fax");
					xyprintf(10 * mapDataStore.mapSizeX, 10 * mapDataStore.mapSizeY, "id: %02d", maps[mapSelected].id);
					xyprintf(10 * mapDataStore.mapSizeX, 40 * mapDataStore.mapSizeY, "%s", maps[mapSelected].chiname.c_str());
					setfont(30 * mapDataStore.mapSizeY, 0, "Lucida Fax");
					xyprintf(300 * mapDataStore.mapSizeX, 40 * mapDataStore.mapSizeY, "%s", maps[mapSelected].engname.c_str());
					xyprintf(10 * mapDataStore.mapSizeX, 70 * mapDataStore.mapSizeY, "Author of the Map: %s", maps[mapSelected].auth.c_str());
					xyprintf(10 * mapDataStore.mapSizeX, 100 * mapDataStore.mapSizeY, "Size of the Map: %d * %d", maps[mapSelected].hei, maps[mapSelected].wid);
					xyprintf(10 * mapDataStore.mapSizeX, 130 * mapDataStore.mapSizeY, "GeneralCount : %d          PlainCount: %d", maps[mapSelected].generalcnt, maps[mapSelected].plaincnt);
					xyprintf(10 * mapDataStore.mapSizeX, 160 * mapDataStore.mapSizeY, "MountainCount: %d          CityCount : %d", maps[mapSelected].mountaincnt, maps[mapSelected].citycnt);
					setcolor(MAGENTA);
					xyprintf(10 * mapDataStore.mapSizeX, 200 * mapDataStore.mapSizeY, "Please Input the Height of the Map (<=500)");
					xyprintf(10 * mapDataStore.mapSizeX, 230 * mapDataStore.mapSizeY, "%d", height);
					xyprintf(810 * mapDataStore.mapSizeX, 200 * mapDataStore.mapSizeY, "Please Input the Width of the Map (<=500)");
					xyprintf(810 * mapDataStore.mapSizeX, 230 * mapDataStore.mapSizeY, "%d", width);
				}
			}
			long long armyMin = 0, armyMax = 0;
			if(mapSelected == 3) {
				setfont(30 * mapDataStore.mapSizeY, 0, "Lucida Fax");
				setcolor(MAGENTA);
				key_msg msg;
				xyprintf(10 * mapDataStore.mapSizeX, 270 * mapDataStore.mapSizeY, "Please Input the Minimum Number of Army on each Block");
				bool isPositive = true;
				while(1) {
					msg = getkey();
					if(msg.msg == key_msg_char) {
						if(msg.key == 13)
							break;
						if(msg.key == 8)
							armyMin /= 10;
						else if(msg.key >= '0' && msg.key <= '9')
							armyMin = armyMin * 10 + msg.key - '0';
						else if(msg.key == '-')
							isPositive = !isPositive;
						cleardevice();
						setcolor(GREEN);
						setfont(40 * mapDataStore.mapSizeY, 0, "Lucida Fax");
						xyprintf(10 * mapDataStore.mapSizeX, 10 * mapDataStore.mapSizeY, "id: %02d", maps[mapSelected].id);
						xyprintf(10 * mapDataStore.mapSizeX, 40 * mapDataStore.mapSizeY, "%s", maps[mapSelected].chiname.c_str());
						setfont(30 * mapDataStore.mapSizeY, 0, "Lucida Fax");
						xyprintf(300 * mapDataStore.mapSizeX, 40 * mapDataStore.mapSizeY, "%s", maps[mapSelected].engname.c_str());
						xyprintf(10 * mapDataStore.mapSizeX, 70 * mapDataStore.mapSizeY, "Author of the Map: %s", maps[mapSelected].auth.c_str());
						xyprintf(10 * mapDataStore.mapSizeX, 100 * mapDataStore.mapSizeY, "Size of the Map: %d * %d", maps[mapSelected].hei, maps[mapSelected].wid);
						xyprintf(10 * mapDataStore.mapSizeX, 130 * mapDataStore.mapSizeY, "GeneralCount : %d          PlainCount: %d", maps[mapSelected].generalcnt, maps[mapSelected].plaincnt);
						xyprintf(10 * mapDataStore.mapSizeX, 160 * mapDataStore.mapSizeY, "MountainCount: %d          CityCount : %d", maps[mapSelected].mountaincnt, maps[mapSelected].citycnt);
						setcolor(MAGENTA);
						xyprintf(10 * mapDataStore.mapSizeX, 200 * mapDataStore.mapSizeY, "Please Input the Height of the Map (<=500)");
						xyprintf(10 * mapDataStore.mapSizeX, 230 * mapDataStore.mapSizeY, "%d", height);
						xyprintf(810 * mapDataStore.mapSizeX, 200 * mapDataStore.mapSizeY, "Please Input the Width of the Map (<=500)");
						xyprintf(810 * mapDataStore.mapSizeX, 230 * mapDataStore.mapSizeY, "%d", width);
						xyprintf(10 * mapDataStore.mapSizeX, 270 * mapDataStore.mapSizeY, "Please Input the Minimum Number of Army on each Block");
						long long realarmy = armyMin;
						if(!isPositive)
							realarmy = -realarmy;
						xyprintf(10 * mapDataStore.mapSizeX, 300 * mapDataStore.mapSizeY, "%lld", realarmy);
					}
				}
				if(!isPositive)
					armyMin = -armyMin;
				cleardevice();
				setcolor(GREEN);
				setfont(40 * mapDataStore.mapSizeY, 0, "Lucida Fax");
				xyprintf(10 * mapDataStore.mapSizeX, 10 * mapDataStore.mapSizeY, "id: %02d", maps[mapSelected].id);
				xyprintf(10 * mapDataStore.mapSizeX, 40 * mapDataStore.mapSizeY, "%s", maps[mapSelected].chiname.c_str());
				setfont(30 * mapDataStore.mapSizeY, 0, "Lucida Fax");
				xyprintf(300 * mapDataStore.mapSizeX, 40 * mapDataStore.mapSizeY, "%s", maps[mapSelected].engname.c_str());
				xyprintf(10 * mapDataStore.mapSizeX, 70 * mapDataStore.mapSizeY, "Author of the Map: %s", maps[mapSelected].auth.c_str());
				xyprintf(10 * mapDataStore.mapSizeX, 100 * mapDataStore.mapSizeY, "Size of the Map: %d * %d", maps[mapSelected].hei, maps[mapSelected].wid);
				xyprintf(10 * mapDataStore.mapSizeX, 130 * mapDataStore.mapSizeY, "GeneralCount : %d          PlainCount: %d", maps[mapSelected].generalcnt, maps[mapSelected].plaincnt);
				xyprintf(10 * mapDataStore.mapSizeX, 160 * mapDataStore.mapSizeY, "MountainCount: %d          CityCount : %d", maps[mapSelected].mountaincnt, maps[mapSelected].citycnt);
				setcolor(MAGENTA);
				xyprintf(10 * mapDataStore.mapSizeX, 200 * mapDataStore.mapSizeY, "Please Input the Height of the Map (<=500)");
				xyprintf(10 * mapDataStore.mapSizeX, 230 * mapDataStore.mapSizeY, "%d", height);
				xyprintf(810 * mapDataStore.mapSizeX, 200 * mapDataStore.mapSizeY, "Please Input the Width of the Map (<=500)");
				xyprintf(810 * mapDataStore.mapSizeX, 230 * mapDataStore.mapSizeY, "%d", width);
				xyprintf(10 * mapDataStore.mapSizeX, 270 * mapDataStore.mapSizeY, "Please Input the Minimum Number of Army on each Block");
				xyprintf(10 * mapDataStore.mapSizeX, 300 * mapDataStore.mapSizeY, "%lld", armyMin);
				xyprintf(810 * mapDataStore.mapSizeX, 270 * mapDataStore.mapSizeY, "Please Input the Maximum Number of Army on each Block");
				isPositive = true;
				while(1) {
					msg = getkey();
					if(msg.msg == key_msg_char) {
						if(msg.key == 13 && armyMax * (isPositive ? 1 : (-1)) >= armyMin)
							break;
						if(msg.key == 8)
							armyMax /= 10;
						else if(msg.key >= '0' && msg.key <= '9')
							armyMax = armyMax * 10 + msg.key - '0';
						else if(msg.key == '-')
							isPositive = !isPositive;
						cleardevice();
						setcolor(GREEN);
						setfont(40 * mapDataStore.mapSizeY, 0, "Lucida Fax");
						xyprintf(10 * mapDataStore.mapSizeX, 10 * mapDataStore.mapSizeY, "id: %02d", maps[mapSelected].id);
						xyprintf(10 * mapDataStore.mapSizeX, 40 * mapDataStore.mapSizeY, "%s", maps[mapSelected].chiname.c_str());
						setfont(30 * mapDataStore.mapSizeY, 0, "Lucida Fax");
						xyprintf(300 * mapDataStore.mapSizeX, 40 * mapDataStore.mapSizeY, "%s", maps[mapSelected].engname.c_str());
						xyprintf(10 * mapDataStore.mapSizeX, 70 * mapDataStore.mapSizeY, "Author of the Map: %s", maps[mapSelected].auth.c_str());
						xyprintf(10 * mapDataStore.mapSizeX, 100 * mapDataStore.mapSizeY, "Size of the Map: %d * %d", maps[mapSelected].hei, maps[mapSelected].wid);
						xyprintf(10 * mapDataStore.mapSizeX, 130 * mapDataStore.mapSizeY, "GeneralCount : %d          PlainCount: %d", maps[mapSelected].generalcnt, maps[mapSelected].plaincnt);
						xyprintf(10 * mapDataStore.mapSizeX, 160 * mapDataStore.mapSizeY, "MountainCount: %d          CityCount : %d", maps[mapSelected].mountaincnt, maps[mapSelected].citycnt);
						setcolor(MAGENTA);
						xyprintf(10 * mapDataStore.mapSizeX, 200 * mapDataStore.mapSizeY, "Please Input the Height of the Map (<=500)");
						xyprintf(10 * mapDataStore.mapSizeX, 230 * mapDataStore.mapSizeY, "%d", height);
						xyprintf(810 * mapDataStore.mapSizeX, 200 * mapDataStore.mapSizeY, "Please Input the Width of the Map (<=500)");
						xyprintf(810 * mapDataStore.mapSizeX, 230 * mapDataStore.mapSizeY, "%d", width);
						xyprintf(10 * mapDataStore.mapSizeX, 270 * mapDataStore.mapSizeY, "Please Input the Minimum Number of Army on each Block");
						xyprintf(10 * mapDataStore.mapSizeX, 300 * mapDataStore.mapSizeY, "%lld", armyMin);
						xyprintf(810 * mapDataStore.mapSizeX, 270 * mapDataStore.mapSizeY, "Please Input the Maximum Number of Army on each Block");
						long long realarmy = armyMax;
						if(!isPositive)
							realarmy = -realarmy;
						xyprintf(810 * mapDataStore.mapSizeX, 300 * mapDataStore.mapSizeY, "%lld", realarmy);
					}
				}
				if(!isPositive)
					armyMax = -armyMax;
				cleardevice();
				setcolor(GREEN);
				setfont(40 * mapDataStore.mapSizeY, 0, "Lucida Fax");
				xyprintf(10 * mapDataStore.mapSizeX, 10 * mapDataStore.mapSizeY, "id: %02d", maps[mapSelected].id);
				xyprintf(10 * mapDataStore.mapSizeX, 40 * mapDataStore.mapSizeY, "%s", maps[mapSelected].chiname.c_str());
				setfont(30 * mapDataStore.mapSizeY, 0, "Lucida Fax");
				xyprintf(300 * mapDataStore.mapSizeX, 40 * mapDataStore.mapSizeY, "%s", maps[mapSelected].engname.c_str());
				xyprintf(10 * mapDataStore.mapSizeX, 70 * mapDataStore.mapSizeY, "Author of the Map: %s", maps[mapSelected].auth.c_str());
				xyprintf(10 * mapDataStore.mapSizeX, 100 * mapDataStore.mapSizeY, "Size of the Map: %d * %d", maps[mapSelected].hei, maps[mapSelected].wid);
				xyprintf(10 * mapDataStore.mapSizeX, 130 * mapDataStore.mapSizeY, "GeneralCount : %d          PlainCount: %d", maps[mapSelected].generalcnt, maps[mapSelected].plaincnt);
				xyprintf(10 * mapDataStore.mapSizeX, 160 * mapDataStore.mapSizeY, "MountainCount: %d          CityCount : %d", maps[mapSelected].mountaincnt, maps[mapSelected].citycnt);
				setcolor(MAGENTA);
				xyprintf(10 * mapDataStore.mapSizeX, 200 * mapDataStore.mapSizeY, "Please Input the Height of the Map (<=500)");
				xyprintf(10 * mapDataStore.mapSizeX, 230 * mapDataStore.mapSizeY, "%d", height);
				xyprintf(810 * mapDataStore.mapSizeX, 200 * mapDataStore.mapSizeY, "Please Input the Width of the Map (<=500)");
				xyprintf(810 * mapDataStore.mapSizeX, 230 * mapDataStore.mapSizeY, "%d", width);
				xyprintf(10 * mapDataStore.mapSizeX, 270 * mapDataStore.mapSizeY, "Please Input the Minimum Number of Army on each Block");
				xyprintf(10 * mapDataStore.mapSizeX, 300 * mapDataStore.mapSizeY, "%lld", armyMin);
				xyprintf(810 * mapDataStore.mapSizeX, 270 * mapDataStore.mapSizeY, "Please Input the Maximum Number of Army on each Block");
				xyprintf(810 * mapDataStore.mapSizeX, 300 * mapDataStore.mapSizeY, "%lld", armyMax);
			}
			importGameSettings();
			switch(mapSelected) {
				case 1:
					createRandomMap(height, width);
					break;
				case 2:
					createStandardMap(height, width);
					break;
				case 3:
					createFullCityMap(height, width, armyMin, armyMax, plCnt);
					break;
				case 4:
					createFullSwampMap(height, width, plCnt);
					break;
				case 5:
					createFullPlainMap(height, width, plCnt);
					break;
			}
		} else {
			cleardevice();
			setcolor(GREEN);
			setfont(40 * mapDataStore.mapSizeY, 0, "Lucida Fax");
			xyprintf(10 * mapDataStore.mapSizeX, 10 * mapDataStore.mapSizeY, "id: %02d", maps[mapSelected].id);
			xyprintf(10 * mapDataStore.mapSizeX, 40 * mapDataStore.mapSizeY, "%s", maps[mapSelected].chiname.c_str());
			setfont(30 * mapDataStore.mapSizeY, 0, "Lucida Fax");
			xyprintf(300 * mapDataStore.mapSizeX, 40 * mapDataStore.mapSizeY, "%s", maps[mapSelected].engname.c_str());
			xyprintf(10 * mapDataStore.mapSizeX, 70 * mapDataStore.mapSizeY, "Author of the Map: %s", maps[mapSelected].auth.c_str());
			xyprintf(10 * mapDataStore.mapSizeX, 100 * mapDataStore.mapSizeY, "Size of the Map: %d * %d", maps[mapSelected].hei, maps[mapSelected].wid);
			xyprintf(10 * mapDataStore.mapSizeX, 130 * mapDataStore.mapSizeY, "GeneralCount : %d          PlainCount: %d", maps[mapSelected].generalcnt, maps[mapSelected].plaincnt);
			xyprintf(10 * mapDataStore.mapSizeX, 160 * mapDataStore.mapSizeY, "MountainCount: %d          CityCount : %d", maps[mapSelected].mountaincnt, maps[mapSelected].citycnt);
			importGameSettings();
			readMap(mapSelected);
		}
		GAME(0, cheatCode, plCnt, stDel);
		exit(0);
	}

	void doMapImport() {
		cleardevice();
		setcolor(BLUE);
		setfont(30 * mapDataStore.mapSizeY, 0, "Lucida Fax");
		xyprintf(10 * mapDataStore.mapSizeX, 10 * mapDataStore.mapSizeY, "Please Input Filename with suffix (end with enter)");
		key_msg msg;
		while(1) {
			msg = getkey();
			if(msg.msg == key_msg_char) {
				if(msg.key == 13)
					break;
				if(msg.key == 8)
					fileName.pop_back();
				else
					fileName += (char)(msg.key);
				cleardevice();
				setcolor(BLUE);
				setfont(30 * mapDataStore.mapSizeY, 0, "Lucida Fax");
				xyprintf(10 * mapDataStore.mapSizeX, 10 * mapDataStore.mapSizeY, "Please Input Filename with suffix (end with enter)");
				xyprintf(10 * mapDataStore.mapSizeX, 110 * mapDataStore.mapSizeY, "%s", fileName.c_str());
			}
			if(msg.msg == key_enter)
				break;
		}
		toAvoidCEBugInGraphicsImportMap(fileName);
	}

	bool cheatCodeSelected[13];

	void importGameSettings() {
		int circlePos = 450 * mapDataStore.mapSizeX;
		PIMAGE refreshCopy = newimage();
		getimage(refreshCopy, 0, 0, 1600 * mapDataStore.mapSizeX, 340 * mapDataStore.mapSizeY);
		setfont(20 * mapDataStore.mapSizeY, 0, "Lucida Fax");
		setcolor(BLUE);
		bool changeMade = true;
		int mouseDownCount = 0;
		bool endConfig = false;
		cheatCode = 2;
		cheatCodeSelected[1] = true;
		for(; is_run(); delay_fps(120)) {
			if(changeMade) {
				cleardevice();
				putimage(0, 0, refreshCopy);
				setcolor(BLUE);
				rectprintf(60 * mapDataStore.mapSizeX, 350 * mapDataStore.mapSizeY, 430 * mapDataStore.mapSizeX, 100 * mapDataStore.mapSizeY, "Please Select The Number of Players");
				for(int i = 1; i <= 12; i++) {
					int lineNumber = (i + 2) / 3;
					int columnNumber = i - (lineNumber - 1) * 3;
					if(i == plCnt) {
						setcolor(RED);
						ege_circle((100 * columnNumber + 50) * mapDataStore.mapSizeX, (500 + 100 * (lineNumber - 1)) * mapDataStore.mapSizeY, 50 * mapDataStore.mapSizeX);
						setcolor(BLUE);
					}
					xyprintf((100 * columnNumber + 50) * mapDataStore.mapSizeX, (500 + 100 * (lineNumber - 1)) * mapDataStore.mapSizeY, "%d", i);
					rectangle(((columnNumber - 1) * 100 + 100) * mapDataStore.mapSizeX, ((lineNumber - 1) * 100 + 450) * mapDataStore.mapSizeY, (columnNumber * 100 + 100) * mapDataStore.mapSizeX, (lineNumber * 100 + 450) * mapDataStore.mapSizeY);
				}
				rectprintf(560 * mapDataStore.mapSizeX, 350 * mapDataStore.mapSizeY, 380, 100, "Drag to Select the Speed of the Game");
				setfillcolor(LIGHTGRAY);
				ege_fillrect(725 * mapDataStore.mapSizeX, 450 * mapDataStore.mapSizeY, 50 * mapDataStore.mapSizeX, 400 * mapDataStore.mapSizeY);
				setfillcolor(BLUE);
				ege_fillellipse(720 * mapDataStore.mapSizeX, circlePos - 30 * mapDataStore.mapSizeY, 60 * mapDataStore.mapSizeX, 60 * mapDataStore.mapSizeY);
				rectprintf(1060 * mapDataStore.mapSizeX, 350 * mapDataStore.mapSizeY, 380 * mapDataStore.mapSizeX, 100 * mapDataStore.mapSizeY, "Select the Players you want to watch Directly");
				for(int i = 1; i <= 12; i++) {
					int lineNumber = (i + 2) / 3;
					int columnNumber = i - (lineNumber - 1) * 3;
					setcolor(BLUE);
					rectangle(((columnNumber - 1) * 100 + 1100) * mapDataStore.mapSizeX, ((lineNumber - 1) * 100 + 450) * mapDataStore.mapSizeY, (columnNumber * 100 + 1100) * mapDataStore.mapSizeX, (lineNumber * 100 + 450) * mapDataStore.mapSizeY);
				}
				for(int i = 1; i <= plCnt; i++) {
					int lineNumber = (i + 2) / 3;
					int columnNumber = i - (lineNumber - 1) * 3;
					setcolor(defTeams[i].color);
					rectprintf(((columnNumber - 1) * 100 + 1100) * mapDataStore.mapSizeX, ((lineNumber - 1) * 100 + 450) * mapDataStore.mapSizeY, 100 * mapDataStore.mapSizeX, 100 * mapDataStore.mapSizeY, "%s", defTeams[i].name.c_str());
					if(cheatCodeSelected[i]) {
						setcolor(RED);
						ege_circle((1050 + 100 * columnNumber) * mapDataStore.mapSizeX, (500 + 100 * (lineNumber - 1)) * mapDataStore.mapSizeY, 50 * mapDataStore.mapSizeX);
						setcolor(BLUE);
					}
				}
				rectangle(900 * mapDataStore.mapSizeX, 30 * mapDataStore.mapSizeY, 1100 * mapDataStore.mapSizeX, 105 * mapDataStore.mapSizeY);
				setfont(70 * mapDataStore.mapSizeY, 0, "Freestyle Script");
				rectprintf(900 * mapDataStore.mapSizeX, 30 * mapDataStore.mapSizeY, 200 * mapDataStore.mapSizeX, 75 * mapDataStore.mapSizeY, "Start Game!");
				setfont(20 * mapDataStore.mapSizeY, 0, "Lucida Fax");
				if(stDel != 0)
					xyprintf(560 * mapDataStore.mapSizeX, 875 * mapDataStore.mapSizeY, "Speed Now: %d", stDel);
				else
					xyprintf(560 * mapDataStore.mapSizeX, 875 * mapDataStore.mapSizeY, "Speed Now: MAXSPEED");
				changeMade = false;
			}
			while(mousemsg()) {
				mouse_msg msg = getmouse();
				// cout << msg.x << ' ' << msg.y << ' ' << msg.is_left() << ' ' << msg.is_down() << ' ' << msg.is_up() << ' ' << msg.is_move() << ' ' << mouseDownCount << ' ' << circlePos << endl;
				if(100 * mapDataStore.mapSizeX <= msg.x && msg.x <= 400 * mapDataStore.mapSizeX && 450 * mapDataStore.mapSizeY <= msg.y && 850 * mapDataStore.mapSizeY >= msg.y) {
					if(msg.is_left() && msg.is_down()) {
						int col = (msg.x + 49 - 50 * mapDataStore.mapSizeX) / (100 * mapDataStore.mapSizeX);
						int lin = (msg.y - 450 * mapDataStore.mapSizeY + 99) / (100 * mapDataStore.mapSizeY);
						plCnt = (lin - 1) * 3 + col;
						changeMade = true;
					}
				}
				if(msg.is_left() && msg.is_down())
					mouseDownCount++;
				if(msg.is_left() && msg.is_up() && mouseDownCount > 0)
					mouseDownCount--;
				if(msg.x >= 720 * mapDataStore.mapSizeX && msg.x <= 780 * mapDataStore.mapSizeX && msg.y >= 450 * mapDataStore.mapSizeY && msg.y <= 850 * mapDataStore.mapSizeY) {
					if(mouseDownCount > 0) {
						circlePos = msg.y;
						if(circlePos >= 840 * mapDataStore.mapSizeY)
							stDel = 0;
						else
							stDel = ((circlePos - 450 * mapDataStore.mapSizeX) / (20 * mapDataStore.mapSizeX)) + 1;
						changeMade = true;
					}
				}
				if(1100 * mapDataStore.mapSizeX <= msg.x && 1400 * mapDataStore.mapSizeX >= msg.x && 450 * mapDataStore.mapSizeY <= msg.y && msg.y <= 850 * mapDataStore.mapSizeY) {
					if(msg.is_left() && msg.is_down()) {
						int col = (msg.x + (99 - 1100) * mapDataStore.mapSizeX) / (100 * mapDataStore.mapSizeX);
						int lin = (msg.y + (99 - 450) * mapDataStore.mapSizeY) / (100 * mapDataStore.mapSizeY);
						int num = (lin - 1) * 3 + col;
						if(num <= plCnt && num > 0) {
							cheatCode ^= (1 << (num));
							cheatCodeSelected[num] ^= 1;
							changeMade = true;
						}
					}
				}
				if(msg.x >= 900 * mapDataStore.mapSizeX && msg.x <= 1100 * mapDataStore.mapSizeX && msg.y >= 30 * mapDataStore.mapSizeY && msg.y <= 105 * mapDataStore.mapSizeY && msg.is_down() && msg.is_left()) {
					endConfig = true;
					cleardevice();
					if(stDel == 0)
						stDel = 120;
					return;
				}
			}
		}
	}

	void init() {
		heightPerBlock = (900.0 * mapDataStore.mapSizeY / (double)mapH);
		widthPerBlock = (900.0 * mapDataStore.mapSizeX / (double)mapW);
		heightPerBlock = widthPerBlock = min(heightPerBlock, widthPerBlock);
		mapDataStore.widthPerBlock = widthPerBlock;
		mapDataStore.heightPerBlock = heightPerBlock;
		setbkmode(TRANSPARENT);
		pimg[1] = newimage();
		getimage(pimg[1], "img/city.png");
		imageOperation::zoomImage(pimg[1], mapDataStore.heightPerBlock, mapDataStore.widthPerBlock);
		pimg[2] = newimage();
		getimage(pimg[2], "img/crown.png");
		imageOperation::zoomImage(pimg[2], mapDataStore.heightPerBlock, mapDataStore.widthPerBlock);
		pimg[3] = newimage();
		getimage(pimg[3], "img/mountain.png");
		imageOperation::zoomImage(pimg[3], mapDataStore.heightPerBlock, mapDataStore.widthPerBlock);
		pimg[4] = newimage();
		getimage(pimg[4], "img/swamp.png");
		imageOperation::zoomImage(pimg[4], mapDataStore.heightPerBlock, mapDataStore.widthPerBlock);
		pimg[5] = newimage();
		getimage(pimg[5], "img/obstacle.png");
		imageOperation::zoomImage(pimg[5], mapDataStore.heightPerBlock, mapDataStore.widthPerBlock);
		pimg[6] = newimage();
		getimage(pimg[6], "img/currentOn.png");
		imageOperation::zoomImage(pimg[6], mapDataStore.heightPerBlock, mapDataStore.widthPerBlock);
		for(int i = 1; i <= 6; i++)
			ege_enable_aa(true, pimg[i]);
		ege_enable_aa(true);
		//		initgraph(1600 * mapDataStore.mapSize, 900 * mapDataStore.mapSize, INIT_RENDERMANUAL);
		setbkcolor(BLACK);
		setbkcolor_f(BLACK);
		cleardevice();
	}
}

namespace LGlocal{
	
};

namespace LGclient{
	
};

namespace LGserver{
	
};

int getHeightPerBlock() {
	return LGGraphics::mapDataStore.heightPerBlock;
}

int getWidthPerBlock() {
	return LGGraphics::mapDataStore.widthPerBlock;
}

#endif