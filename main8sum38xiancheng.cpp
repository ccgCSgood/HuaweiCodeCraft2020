#include <iostream>
#include<vector>
#include<queue>
#include<string>
#include<algorithm>
#include <thread>
#include<unordered_map>
#include<ctime>
//#include<unistd.h>

using namespace std;

typedef unsigned long long ull;

const unsigned int nodemax = 600001;//edge1里的长度  400w 可以缩减
const unsigned int node400w = 4000001;//edge1里的长度 必须大于400w
const unsigned int hu2max = 1000000;//maxdu^2 2000w  80M内存  现在是100*100*100=100w
const unsigned int zy23 = 23;// 原左移32位  上线400w 左移22位即可
const unsigned int du100 = 100;// 数组可以存储的最大出度入度

unsigned int dataidnum;//输入文本id数
unsigned int allline;
unsigned int line3, line4, line5, line6, line7;
unsigned int line31, line41, line51, line61, line71;
unsigned int line32, line42, line52, line62, line72;
unsigned int line33, line43, line53, line63, line73;
unsigned int line34, line44, line54, line64, line74;
unsigned int line35, line45, line55, line65, line75;
unsigned int line36, line46, line56, line66, line76;
unsigned int line37, line47, line57, line67, line77;
unsigned int line38, line48, line58, line68, line78;

vector<unsigned int>inputs;//载入数据往里推
unsigned int inputs2[node400w];//载入数据往里推
ull inputs3[node400w];//暂存money
unsigned int newinputs[node400w];//载入数据往里推

unsigned int nodeCnt, V;//  nodeCnt最大节点数， V为最大节点的id

unsigned int edge1[nodemax][du100];
unsigned int edge2[nodemax][du100];
vector<vector<unsigned int>> edge1v, edge2v;

unsigned int edge1m[nodemax][du100];
unsigned int edge2m[nodemax][du100];
vector<vector<unsigned int>> edge1vm, edge2vm;

unordered_map<unsigned int, unsigned int> idHash; //ys;
unordered_map<ull, unsigned int> money;

unsigned int in[nodemax];
unsigned int out[nodemax];
unsigned int rudu[nodemax];
unsigned int chudu[nodemax];

unsigned int inin[nodemax];
unsigned int outout[nodemax];//用来写入链表计数用

bool vis[nodemax];
string  hui[node400w];
string  hui2[node400w];

unsigned int hu2[hu2max][6];
unsigned int hu22[hu2max][6];
unsigned int hu222[hu2max][6];
unsigned int hu2222[hu2max][6];
unsigned int hu22222[hu2max][6];
unsigned int hu222222[hu2max][6];
unsigned int hu2222222[hu2max][6];
unsigned int hu22222222[hu2max][6];
string  sch3, sch33, sch333, sch3333, sch33333, sch333333, sch3333333, sch33333333;
string  sch4, sch44, sch444, sch4444, sch44444, sch444444, sch4444444, sch44444444;
string  sch5, sch55, sch555, sch5555, sch55555, sch555555, sch5555555, sch55555555;
string  sch6, sch66, sch666, sch6666, sch66666, sch666666, sch6666666, sch66666666;
string  sch7, sch77, sch777, sch7777, sch77777, sch777777, sch7777777, sch77777777;

bool mark3[nodemax], mark33[nodemax], mark333[nodemax], mark3333[nodemax];
bool mark33333[nodemax], mark333333[nodemax], mark3333333[nodemax], mark33333333[nodemax];






void duwenjian(string& testFile) {
	FILE* file = fopen(testFile.c_str(), "r");
	unsigned int u, v, c;
	while (fscanf(file, "%d,%d,%d", &u, &v, &c) != EOF) {
		inputs.emplace_back(u);
		inputs.emplace_back(v);
		inputs2[dataidnum] = u;
		inputs2[dataidnum + 1] = v;
		inputs3[dataidnum] = c;
		dataidnum += 2;
	}
}

void newGraph() {

	sort(inputs.begin(), inputs.end());
	inputs.erase(unique(inputs.begin(), inputs.end()), inputs.end());
	nodeCnt = inputs.size();//node num
	V = inputs[nodeCnt - 1];// max id
	nodeCnt = 0;
	for (unsigned int& x : inputs) {
		hui[nodeCnt] = to_string(x) + ',';
		hui2[nodeCnt] = to_string(x) + "\r\n";//映射
		idHash[x] = nodeCnt++;
	}

	edge1v.resize(nodeCnt);
	edge2v.resize(nodeCnt);

	unsigned int u, v;
	for (int i = 0; i < dataidnum; i += 2) {

		u = idHash[inputs2[i]], v = idHash[inputs2[i + 1]];
		newinputs[i] = u;
		newinputs[i + 1] = v;
		//edge1v[u].emplace_back(v);
		//edge2v[v].emplace_back(u);

		++in[v]; ++rudu[v];
		++out[u]; ++chudu[u];

		money[(ull)u << zy23 | v] = inputs3[i];
	}

	u = 0; v = 0;
	for (int i = 0; i < dataidnum; i += 2) {
		u = newinputs[i];
		v = newinputs[i + 1];

		if (out[u] < du100) {
			edge1[u][outout[u]] = v;
			outout[u]++;
		}
		else {
			edge1v[u].emplace_back(v);
		}

		if (in[v] < du100) {
			edge2[v][inin[v]] = u;
			inin[v]++;
		}
		else {
			edge2v[v].emplace_back(u);
		}
	}

}
//动态切换写入链表；出度入度100以内的写入edge1 edge2;

void diansort() {
	queue<unsigned int> du;
	for (unsigned int i = 0; i < nodeCnt; i++) {
		if (0 == chudu[i])
			du.push(i);
	}
	while (!du.empty()) {
		int u = du.front();
		du.pop();
		unsigned int v = 0;
		for (unsigned int vii = 0; vii < in[u]; vii++) {
			if (in[u] < du100) {
				v = edge2[u][vii];
			}
			else v = edge2v[u][vii];

			if (0 == --chudu[v])
				du.push(v);
		}
	}

	for (int i = 0; i < nodeCnt; i++) {
		if (chudu[i] == 0) {
			vis[i] = true;
		}
		if (chudu[i] > 1) {
			if (out[i] < du100) {
				sort(edge1[i], edge1[i] + out[i]);
			}
			else {
				sort(edge1v[i].begin(), edge1v[i].end());
			}

		}
	}
}

void dian2sort() {
	queue<unsigned int> du;
	for (unsigned int i = 0; i < nodeCnt; i++) {
		if (0 == rudu[i])
			du.push(i);
	}
	while (!du.empty()) {
		int u = du.front();
		du.pop();
		unsigned int v;
		for (unsigned int vi = 0; vi < out[u]; vi++) {
			if (out[u] < du100) {
				v = edge1[u][vi];
			}
			else v = edge1v[u][vi];

			if (0 == --rudu[v])
				du.push(v);
		}
	}
	for (int i = 0; i < nodeCnt; i++) {
		if (rudu[i] == 0) {
			vis[i] = true;
		}
		if (rudu[i] > 1) {
			if (in[i] < du100) {
				sort(edge2[i], edge2[i] + in[i]);
			}
			else {
				sort(edge2v[i].begin(), edge2v[i].end());
			}

		}

	}

}

void qian() {
	edge1vm.resize(nodeCnt);
	edge2vm.resize(nodeCnt);

	for (int i = 0; i < nodeCnt; i++) {
		for (int j = 0; j < out[i]; j++) {
			if (out[i] < du100) {
				edge1m[i][j] = (money[ull(i) << zy23 | edge1[i][j]]);
			}
			else {
				edge1vm[i].emplace_back(money[ull(i) << zy23 | edge1v[i][j]]);
			}
			for (int k = 0; k < in[i]; k++) {
				if (in[i] < du100) {
					edge2m[i][k] = (money[ull(edge2[i][k]) << zy23 | i]);
				}
				else {
					edge2vm[i].emplace_back(money[ull(edge2v[i][k]) << zy23 | i]);
				}
			}
		}
	}
}

void findcycle1(int q1, int end1) {
	//queue<unsigned int> du1;
	unsigned int v11 = 0, v21 = 0, v31 = 0, v41 = 0, v51 = 0, u11 = 0, u21 = 0, u31 = 0;
	ull c1 = 0, c2 = 0, c3 = 0, c4 = 0, c5 = 0, c6 = 0, c7 = 0;
	ull b3c1 = 0, b3c2 = 0, b3c3 = 0, b3c4 = 0, b3c5 = 0, b3c6 = 0, b3c7 = 0;
	ull b5c1 = 0, b5c2 = 0, b5c3 = 0, b5c4 = 0, b5c5 = 0, b5c6 = 0, b5c7 = 0;

	while (q1 < end1) {
		if (vis[q1]) { q1++; continue; }

		unsigned int  k221 = 0;

		for (unsigned int ii11 = 0; ii11 < in[q1]; ii11++) {
			if (in[q1] < du100) {
				u11 = edge2[q1][ii11];
			}
			else {
				u11 = edge2v[q1][ii11];
			}

			if ((u11 <= q1) || (vis[u11]))continue;

			if (in[q1] < du100) {
				c7 = edge2m[q1][ii11];
			}
			else {
				c7 = edge2vm[q1][ii11];
			}

			b5c7 = 5 * c7;

			for (unsigned int ii21 = 0; ii21 < in[u11]; ii21++) {
				if (in[u11] < du100) {
					u21 = edge2[u11][ii21];
				}
				else {
					u21 = edge2v[u11][ii21];
				}

				if ((u21 <= q1) || (vis[u21] == true) || (u21 == u11))continue;

				if (in[u11] < du100) {
					c6 = edge2m[u11][ii21];
				}
				else {
					c6 = edge2vm[u11][ii21];
				}
				if ((b5c7 < c6) || (c7 > 3 * c6)) continue;

				b5c6 = 5 * c6;

				for (unsigned int ii31 = 0; ii31 < in[u21]; ii31++) {
					if (in[u21] < du100) {
						u31 = edge2[u21][ii31];
					}
					else {
						u31 = edge2v[u21][ii31];
					}

					if ((u31 <= q1) || (vis[u31] == true) || (u31 == u11) || (u31 == u21))continue;

					if (in[u21] < du100) {
						c5 = edge2m[u21][ii31];
					}
					else {
						c5 = edge2vm[u21][ii31];
					}
					if ((b5c6 < c5) || (c6 > 3 * c5)) continue;

					mark3[u31] = true;

					hu2[k221][0] = u11;
					hu2[k221][1] = u21;
					hu2[k221][2] = u31;
					hu2[k221][3] = c7;
					hu2[k221][4] = c6;
					hu2[k221][5] = c5;

					k221++;

				}
			}
		}
		for (int c = 0; c < k221; c++) {
			for (int ti = c + 1; ti < k221; ti++) {
				if (hu2[c][2] == hu2[ti][2]) {
					if (hu2[c][1] > hu2[ti][1]) swap(hu2[c], hu2[ti]);
					if (hu2[c][1] == hu2[ti][1]) {
						if (hu2[c][0] > hu2[ti][0]) swap(hu2[c], hu2[ti]);
					}
				}
			}
		}
		//排序后3

		for (unsigned int i1 = 0; i1 < out[q1]; i1++) {
			if (out[q1] < du100) {
				v11 = edge1[q1][i1];
			}
			else {
				v11 = edge1v[q1][i1];
			}

			if ((v11 <= q1) || (vis[v11] == true))continue;
			if (out[q1] < du100) {
				c1 = edge1m[q1][i1];
			}
			else {
				c1 = edge1vm[q1][i1];
			}
			b5c1 = 5 * c1;
			b3c1 = 3 * c1;
			/*
			if (mark3[v11]) {
				for (unsigned int j41 = 0; j41 < k221; j41++) {
					if (v11 == hu2[j41][2] && v11 != hu2[j41][1] && v11 != hu2[j41][0]) {
						c5 = hu2[j41][5];
						c7 = hu2[j41][3];
						if ((b5c1 >= c7) && (c1 <= 3*c7)&& (5*c5 >= c1) && (c5 <= b3c1)) {
							sch4 += hui[q1] + hui[v11] + hui[hu2[j41][1]] + hui2[hu2[j41][0]];

							line41++;
						}
					}
				}
			}*/

			for (unsigned int j1 = 0; j1 < out[v11]; j1++) {
				if (out[v11] < du100) {
					v21 = edge1[v11][j1];
				}
				else {
					v21 = edge1v[v11][j1];
				}
				if ((v21 <= q1) || (vis[v21] == true) || (v11 == v21))continue;

				if (out[v11] < du100) {
					c2 = edge1m[v11][j1];
				}
				else {
					c2 = edge1vm[v11][j1];
				}
				b5c2 = 5 * c2;
				if ((b5c2 < c1) || (c2 > b3c1))continue;
				b3c2 = 3 * c2;

				if (mark3[v21]) {
					for (unsigned int j51 = 0; j51 < k221; j51++) {
						if (v21 == hu2[j51][2] && v11 != hu2[j51][1] && v11 != hu2[j51][0]) {
							c5 = hu2[j51][5];
							c7 = hu2[j51][3];
							if ((b5c1 >= c7) && (c1 <= 3 * c7) && (5 * c5 >= c2) && (c5 <= b3c2)) {
								sch5 += hui[q1] + hui[v11] + hui[v21] + hui[hu2[j51][1]] + hui2[hu2[j51][0]];

								line51++;
							}
						}
					}
				}

				for (unsigned int j31 = 0; j31 < out[v21]; j31++) {
					if (out[v21] < du100) {
						v31 = edge1[v21][j31];
					}
					else {
						v31 = edge1v[v21][j31];
					}
					if (v31 < q1 || v11 == v31 || (vis[v31] == true) || (v31 == v21))continue;
					if (out[v21] < du100) {
						c3 = edge1m[v21][j31];
					}
					else {
						c3 = edge1vm[v21][j31];
					}

					b3c2 = 3 * c2;
					b5c3 = 5 * c3;
					if ((b5c3 < c2) || (c3 > b3c2))continue;

					b5c1 = 5 * c1;
					b3c3 = 3 * c3;
					if (v31 == q1) {

						if ((b5c1 >= c3) && (c1 <= b3c3)) {
							sch3 += hui[q1] + hui[v11] + hui2[v21];

							line31++;
						}
						continue;
					}

					if (mark3[v31]) {
						for (unsigned int j61 = 0; j61 < k221; j61++) {
							if (v31 == hu2[j61][2] && v11 != hu2[j61][1] && v11 != hu2[j61][0] && v21 != hu2[j61][1] && v21 != hu2[j61][0]) {
								c5 = hu2[j61][5];
								c7 = hu2[j61][3];
								if ((b5c1 >= c7) && (c1 <= 3 * c7) && (5 * c5 >= c3) && (c5 <= b3c3)) {
									sch6 += hui[q1] + hui[v11] + hui[v21] + hui[v31] + hui[hu2[j61][1]] + hui2[hu2[j61][0]];

									line61++;
								}
							}
						}
					}

					for (unsigned int j41 = 0; j41 < out[v31]; j41++) {

						if (out[v31] < du100) {
							v41 = edge1[v31][j41];
						}
						else {
							v41 = edge1v[v31][j41];
						}

						if ((v41 < q1) || v11 == v41 || v21 == v41 || (vis[v41] == true) || (v41 == v31))continue;
						if (out[v31] < du100) {
							c4 = edge1m[v31][j41];
						}
						else {
							c4 = edge1vm[v31][j41];
						}

						b3c3 = 3 * c3;
						b5c4 = 5 * c4;

						if ((b5c4 < c3) || (c4 > b3c3))continue;
						b5c1 = 5 * c1;
						b3c4 = 3 * c4;

						if (v41 == q1) {

							if ((b5c1 >= c4) && (c1 <= b3c4)) {
								sch4 += hui[q1] + hui[v11] + hui[v21] + hui2[v31];

								line41++;
								continue;
							}
							continue;

						}

						if (mark3[v41] == false) continue;

						if (mark3[v41]) {
							for (unsigned int j71 = 0; j71 < k221; j71++) {
								if (v41 == hu2[j71][2] && v11 != hu2[j71][1] && v11 != hu2[j71][0] && v21 != hu2[j71][1] && v21 != hu2[j71][0] && v31 != hu2[j71][1] && v31 != hu2[j71][0]) {
									c5 = hu2[j71][5];
									c7 = hu2[j71][3];
									if ((b5c1 >= c7) && (c1 <= 3 * c7) && (5 * c5 >= c4) && (c5 <= b3c4)) {
										sch7 += hui[q1] + hui[v11] + hui[v21] + hui[v31] + hui[v41] + hui[hu2[j71][1]] + hui2[hu2[j71][0]];

										line71++;
									}
								}
							}
						}
					}
				}
			}
		}

		for (int im1 = 0; im1 < k221; im1++) {
			mark3[hu2[im1][2]] = false;
		}
		q1++;
	}
}

void findcycle2(int q2, int end2) {

	unsigned int v12 = 0, v22 = 0, v32 = 0, v42 = 0, v52 = 0, u12 = 0, u22 = 0, u32 = 0;
	ull c12 = 0, c22 = 0, c32 = 0, c42 = 0, c52 = 0, c62 = 0, c72 = 0;
	ull b3c12 = 0, b3c22 = 0, b3c32 = 0, b3c42 = 0, b3c52 = 0, b3c62 = 0, b3c72 = 0;
	ull b5c12 = 0, b5c22 = 0, b5c32 = 0, b5c42 = 0, b5c52 = 0, b5c62 = 0, b5c72 = 0;

	while (q2 < end2) {
		if (vis[q2]) { q2++; continue; }

		unsigned int  k222 = 0;

		for (unsigned int ii12 = 0; ii12 < in[q2]; ii12++) {
			if (in[q2] < du100) {
				u12 = edge2[q2][ii12];
			}
			else {
				u12 = edge2v[q2][ii12];
			}

			if ((u12 <= q2) || (vis[u12]))continue;

			if (in[q2] < du100) {
				c72 = edge2m[q2][ii12];
			}
			else {
				c72 = edge2vm[q2][ii12];
			}

			b5c72 = 5 * c72;

			for (unsigned int ii22 = 0; ii22 < in[u12]; ii22++) {
				if (in[u12] < du100) {
					u22 = edge2[u12][ii22];
				}
				else {
					u22 = edge2v[u12][ii22];
				}

				if ((u22 <= q2) || (vis[u22] == true) || (u22 == u12))continue;

				if (in[u12] < du100) {
					c62 = edge2m[u12][ii22];
				}
				else {
					c62 = edge2vm[u12][ii22];
				}
				if ((b5c72 < c62) || (c72 > 3 * c62)) continue;

				b5c62 = 5 * c62;

				for (unsigned int ii32 = 0; ii32 < in[u22]; ii32++) {
					if (in[u22] < du100) {
						u32 = edge2[u22][ii32];
					}
					else {
						u32 = edge2v[u22][ii32];
					}

					if ((u32 <= q2) || (vis[u32] == true) || (u32 == u12) || (u32 == u22))continue;

					if (in[u22] < du100) {
						c52 = edge2m[u22][ii32];
					}
					else {
						c52 = edge2vm[u22][ii32];
					}
					if ((b5c62 < c52) || (c62 > 3 * c52)) continue;

					mark33[u32] = true;

					hu22[k222][0] = u12;
					hu22[k222][1] = u22;
					hu22[k222][2] = u32;
					hu22[k222][3] = c72;
					hu22[k222][4] = c62;
					hu22[k222][5] = c52;

					k222++;

				}
			}
		}
		for (int cz = 0; cz < k222; cz++) {
			for (int tiz = cz + 1; tiz < k222; tiz++) {
				if (hu22[cz][2] == hu22[tiz][2]) {
					if (hu22[cz][1] > hu22[tiz][1]) swap(hu22[cz], hu22[tiz]);
					if (hu22[cz][1] == hu22[tiz][1]) {
						if (hu22[cz][0] > hu22[tiz][0]) swap(hu22[cz], hu22[tiz]);
					}
				}
			}
		}
		//排序后3

		for (unsigned int i2 = 0; i2 < out[q2]; i2++) {
			if (out[q2] < du100) {
				v12 = edge1[q2][i2];
			}
			else {
				v12 = edge1v[q2][i2];
			}

			if ((v12 <= q2) || (vis[v12] == true))continue;
			if (out[q2] < du100) {
				c12 = edge1m[q2][i2];
			}
			else {
				c12 = edge1vm[q2][i2];
			}
			b5c12 = 5 * c12;
			b3c12 = 3 * c12;

			for (unsigned int j2 = 0; j2 < out[v12]; j2++) {
				if (out[v12] < du100) {
					v22 = edge1[v12][j2];
				}
				else {
					v22 = edge1v[v12][j2];
				}
				if ((v22 <= q2) || (vis[v22] == true) || (v12 == v22))continue;

				if (out[v12] < du100) {
					c22 = edge1m[v12][j2];
				}
				else {
					c22 = edge1vm[v12][j2];
				}
				b5c22 = 5 * c22;
				if ((b5c22 < c12) || (c22 > b3c12))continue;
				b3c22 = 3 * c22;

				if (mark33[v22]) {
					for (unsigned int j52 = 0; j52 < k222; j52++) {
						if (v22 == hu22[j52][2] && v12 != hu22[j52][1] && v12 != hu22[j52][0]) {
							c52 = hu22[j52][5];
							c72 = hu22[j52][3];
							if ((b5c12 >= c72) && (c12 <= 3 * c72) && (5 * c52 >= c22) && (c52 <= b3c22)) {
								sch55 += hui[q2] + hui[v12] + hui[v22] + hui[hu22[j52][1]] + hui2[hu22[j52][0]];

								line52++;
							}
						}
					}
				}

				for (unsigned int j32 = 0; j32 < out[v22]; j32++) {
					if (out[v22] < du100) {
						v32 = edge1[v22][j32];
					}
					else {
						v32 = edge1v[v22][j32];
					}
					if (v32 < q2 || v12 == v32 || (vis[v32] == true) || (v32 == v22))continue;
					if (out[v22] < du100) {
						c32 = edge1m[v22][j32];
					}
					else {
						c32 = edge1vm[v22][j32];
					}

					b3c22 = 3 * c22;
					b5c32 = 5 * c32;
					if ((b5c32 < c22) || (c32 > b3c22))continue;

					b5c12 = 5 * c12;
					b3c32 = 3 * c32;
					if (v32 == q2) {

						if ((b5c12 >= c32) && (c12 <= b3c32)) {
							sch33 += hui[q2] + hui[v12] + hui2[v22];

							line32++;
						}
						continue;
					}

					if (mark33[v32]) {
						for (unsigned int j62 = 0; j62 < k222; j62++) {
							if (v32 == hu22[j62][2] && v12 != hu22[j62][1] && v12 != hu22[j62][0] && v22 != hu22[j62][1] && v22 != hu22[j62][0]) {
								c52 = hu22[j62][5];
								c72 = hu22[j62][3];
								if ((b5c12 >= c72) && (c12 <= 3 * c72) && (5 * c52 >= c32) && (c52 <= b3c32)) {
									sch66 += hui[q2] + hui[v12] + hui[v22] + hui[v32] + hui[hu22[j62][1]] + hui2[hu22[j62][0]];

									line62++;
								}
							}
						}
					}

					for (unsigned int j42 = 0; j42 < out[v32]; j42++) {

						if (out[v32] < du100) {
							v42 = edge1[v32][j42];
						}
						else {
							v42 = edge1v[v32][j42];
						}

						if ((v42 < q2) || v12 == v42 || v22 == v42 || (vis[v42] == true) || (v42 == v32))continue;
						if (out[v32] < du100) {
							c42 = edge1m[v32][j42];
						}
						else {
							c42 = edge1vm[v32][j42];
						}

						b3c32 = 3 * c32;
						b5c42 = 5 * c42;

						if ((b5c42 < c32) || (c42 > b3c32))continue;
						b5c12 = 5 * c12;
						b3c42 = 3 * c42;

						if (v42 == q2) {

							if ((b5c12 >= c42) && (c12 <= b3c42)) {
								sch44 += hui[q2] + hui[v12] + hui[v22] + hui2[v32];

								line42++;
								continue;
							}
							continue;

						}

						if (mark33[v42] == false) continue;

						if (mark33[v42]) {
							for (unsigned int j72 = 0; j72 < k222; j72++) {
								if (v42 == hu22[j72][2] && v12 != hu22[j72][1] && v12 != hu22[j72][0] && v22 != hu22[j72][1] && v22 != hu22[j72][0] && v32 != hu22[j72][1] && v32 != hu22[j72][0]) {
									c52 = hu22[j72][5];
									c72 = hu22[j72][3];
									if ((b5c12 >= c72) && (c12 <= 3 * c72) && (5 * c52 >= c42) && (c52 <= b3c42)) {
										sch77 += hui[q2] + hui[v12] + hui[v22] + hui[v32] + hui[v42] + hui[hu22[j72][1]] + hui2[hu22[j72][0]];

										line72++;
									}
								}
							}
						}
					}
				}
			}
		}

		for (int im2 = 0; im2 < k222; im2++) {
			mark33[hu22[im2][2]] = false;
		}
		q2++;
	}
}

void findcycle3(int q3, int end3) {

	unsigned int v13 = 0, v23 = 0, v33 = 0, v43 = 0, v53 = 0, u13 = 0, u23 = 0, u33 = 0;
	ull c13 = 0, c23 = 0, c33 = 0, c43 = 0, c53 = 0, c63 = 0, c73 = 0;
	ull b3c13 = 0, b3c23 = 0, b3c33 = 0, b3c43 = 0, b3c53 = 0, b3c63 = 0, b3c73 = 0;
	ull b5c13 = 0, b5c23 = 0, b5c33 = 0, b5c43 = 0, b5c53 = 0, b5c63 = 0, b5c73 = 0;

	while (q3 < end3) {
		if (vis[q3]) { q3++; continue; }

		unsigned int  k223 = 0;

		for (unsigned int ii13 = 0; ii13 < in[q3]; ii13++) {
			if (in[q3] < du100) {
				u13 = edge2[q3][ii13];
			}
			else {
				u13 = edge2v[q3][ii13];
			}

			if ((u13 <= q3) || (vis[u13]))continue;

			if (in[q3] < du100) {
				c73 = edge2m[q3][ii13];
			}
			else {
				c73 = edge2vm[q3][ii13];
			}

			b5c73 = 5 * c73;

			for (unsigned int ii23 = 0; ii23 < in[u13]; ii23++) {
				if (in[u13] < du100) {
					u23 = edge2[u13][ii23];
				}
				else {
					u23 = edge2v[u13][ii23];
				}

				if ((u23 <= q3) || (vis[u23] == true) || (u23 == u13))continue;

				if (in[u13] < du100) {
					c63 = edge2m[u13][ii23];
				}
				else {
					c63 = edge2vm[u13][ii23];
				}
				if ((b5c73 < c63) || (c73 > 3 * c63)) continue;

				b5c63 = 5 * c63;

				for (unsigned int ii33 = 0; ii33 < in[u23]; ii33++) {
					if (in[u23] < du100) {
						u33 = edge2[u23][ii33];
					}
					else {
						u33 = edge2v[u23][ii33];
					}

					if ((u33 <= q3) || (vis[u33] == true) || (u33 == u13) || (u33 == u23))continue;

					if (in[u23] < du100) {
						c53 = edge2m[u23][ii33];
					}
					else {
						c53 = edge2vm[u23][ii33];
					}
					if ((b5c63 < c53) || (c63 > 3 * c53)) continue;

					mark333[u33] = true;

					hu222[k223][0] = u13;
					hu222[k223][1] = u23;
					hu222[k223][2] = u33;
					hu222[k223][3] = c73;
					hu222[k223][4] = c63;
					hu222[k223][5] = c53;

					k223++;

				}
			}
		}
		for (int cz3 = 0; cz3 < k223; cz3++) {
			for (int tiz3 = cz3 + 1; tiz3 < k223; tiz3++) {
				if (hu222[cz3][2] == hu222[tiz3][2]) {
					if (hu222[cz3][1] > hu222[tiz3][1]) swap(hu222[cz3], hu222[tiz3]);
					if (hu222[cz3][1] == hu222[tiz3][1]) {
						if (hu222[cz3][0] > hu222[tiz3][0]) swap(hu222[cz3], hu222[tiz3]);
					}
				}
			}
		}
		//排序后3

		for (unsigned int i3 = 0; i3 < out[q3]; i3++) {
			if (out[q3] < du100) {
				v13 = edge1[q3][i3];
			}
			else {
				v13 = edge1v[q3][i3];
			}

			if ((v13 <= q3) || (vis[v13] == true))continue;
			if (out[q3] < du100) {
				c13 = edge1m[q3][i3];
			}
			else {
				c13 = edge1vm[q3][i3];
			}
			b5c13 = 5 * c13;
			b3c13 = 3 * c13;

			for (unsigned int j3 = 0; j3 < out[v13]; j3++) {
				if (out[v13] < du100) {
					v23 = edge1[v13][j3];
				}
				else {
					v23 = edge1v[v13][j3];
				}
				if ((v23 <= q3) || (vis[v23] == true) || (v13 == v23))continue;

				if (out[v13] < du100) {
					c23 = edge1m[v13][j3];
				}
				else {
					c23 = edge1vm[v13][j3];
				}
				b5c23 = 5 * c23;
				if ((b5c23 < c13) || (c23 > b3c13))continue;
				b3c23 = 3 * c23;

				if (mark333[v23]) {
					for (unsigned int j53 = 0; j53 < k223; j53++) {
						if (v23 == hu222[j53][2] && v13 != hu222[j53][1] && v13 != hu222[j53][0]) {
							c53 = hu222[j53][5];
							c73 = hu222[j53][3];
							if ((b5c13 >= c73) && (c13 <= 3 * c73) && (5 * c53 >= c23) && (c53 <= b3c23)) {
								sch555 += hui[q3] + hui[v13] + hui[v23] + hui[hu222[j53][1]] + hui2[hu222[j53][0]];

								line53++;
							}
						}
					}
				}

				for (unsigned int j33 = 0; j33 < out[v23]; j33++) {
					if (out[v23] < du100) {
						v33 = edge1[v23][j33];
					}
					else {
						v33 = edge1v[v23][j33];
					}
					if (v33 < q3 || v13 == v33 || (vis[v33] == true) || (v33 == v23))continue;
					if (out[v23] < du100) {
						c33 = edge1m[v23][j33];
					}
					else {
						c33 = edge1vm[v23][j33];
					}

					b3c23 = 3 * c23;
					b5c33 = 5 * c33;
					if ((b5c33 < c23) || (c33 > b3c23))continue;

					b5c13 = 5 * c13;
					b3c33 = 3 * c33;
					if (v33 == q3) {

						if ((b5c13 >= c33) && (c13 <= b3c33)) {
							sch333 += hui[q3] + hui[v13] + hui2[v23];

							line33++;
						}
						continue;
					}

					if (mark333[v33]) {
						for (unsigned int j63 = 0; j63 < k223; j63++) {
							if (v33 == hu222[j63][2] && v13 != hu222[j63][1] && v13 != hu222[j63][0] && v23 != hu222[j63][1] && v23 != hu222[j63][0]) {
								c53 = hu222[j63][5];
								c73 = hu222[j63][3];
								if ((b5c13 >= c73) && (c13 <= 3 * c73) && (5 * c53 >= c33) && (c53 <= b3c33)) {
									sch666 += hui[q3] + hui[v13] + hui[v23] + hui[v33] + hui[hu222[j63][1]] + hui2[hu222[j63][0]];

									line63++;
								}
							}
						}
					}

					for (unsigned int j43 = 0; j43 < out[v33]; j43++) {

						if (out[v33] < du100) {
							v43 = edge1[v33][j43];
						}
						else {
							v43 = edge1v[v33][j43];
						}

						if ((v43 < q3) || v13 == v43 || v23 == v43 || (vis[v43] == true) || (v43 == v33))continue;
						if (out[v33] < du100) {
							c43 = edge1m[v33][j43];
						}
						else {
							c43 = edge1vm[v33][j43];
						}

						b3c33 = 3 * c33;
						b5c43 = 5 * c43;

						if ((b5c43 < c33) || (c43 > b3c33))continue;
						b5c13 = 5 * c13;
						b3c43 = 3 * c43;

						if (v43 == q3) {

							if ((b5c13 >= c43) && (c13 <= b3c43)) {
								sch444 += hui[q3] + hui[v13] + hui[v23] + hui2[v33];

								line43++;
								continue;
							}
							continue;

						}

						if (mark333[v43] == false) continue;

						if (mark333[v43]) {
							for (unsigned int j73 = 0; j73 < k223; j73++) {
								if (v43 == hu222[j73][2] && v13 != hu222[j73][1] && v13 != hu222[j73][0] && v23 != hu222[j73][1] && v23 != hu222[j73][0] && v33 != hu222[j73][1] && v33 != hu222[j73][0]) {
									c53 = hu222[j73][5];
									c73 = hu222[j73][3];
									if ((b5c13 >= c73) && (c13 <= 3 * c73) && (5 * c53 >= c43) && (c53 <= b3c43)) {
										sch777 += hui[q3] + hui[v13] + hui[v23] + hui[v33] + hui[v43] + hui[hu222[j73][1]] + hui2[hu222[j73][0]];

										line73++;
									}
								}
							}
						}
					}
				}
			}
		}

		for (int im3 = 0; im3 < k223; im3++) {
			mark333[hu222[im3][2]] = false;
		}
		q3++;
	}
}

void findcycle4(int q4, int end4) {

	unsigned int v14 = 0, v24 = 0, v34 = 0, v44 = 0, v54 = 0, u14 = 0, u24 = 0, u34 = 0;
	ull c14 = 0, c24 = 0, c34 = 0, c44 = 0, c54 = 0, c64 = 0, c74 = 0;
	ull b3c14 = 0, b3c24 = 0, b3c34 = 0, b3c44 = 0, b3c54 = 0, b3c64 = 0, b3c74 = 0;
	ull b5c14 = 0, b5c24 = 0, b5c34 = 0, b5c44 = 0, b5c54 = 0, b5c64 = 0, b5c74 = 0;

	while (q4 < end4) {
		if (vis[q4]) { q4++; continue; }

		unsigned int  k224 = 0;

		for (unsigned int ii14 = 0; ii14 < in[q4]; ii14++) {
			if (in[q4] < du100) {
				u14 = edge2[q4][ii14];
			}
			else {
				u14 = edge2v[q4][ii14];
			}

			if ((u14 <= q4) || (vis[u14]))continue;

			if (in[q4] < du100) {
				c74 = edge2m[q4][ii14];
			}
			else {
				c74 = edge2vm[q4][ii14];
			}

			b5c74 = 5 * c74;

			for (unsigned int ii24 = 0; ii24 < in[u14]; ii24++) {
				if (in[u14] < du100) {
					u24 = edge2[u14][ii24];
				}
				else {
					u24 = edge2v[u14][ii24];
				}

				if ((u24 <= q4) || (vis[u24] == true) || (u24 == u14))continue;

				if (in[u14] < du100) {
					c64 = edge2m[u14][ii24];
				}
				else {
					c64 = edge2vm[u14][ii24];
				}
				if ((b5c74 < c64) || (c74 > 3 * c64)) continue;

				b5c64 = 5 * c64;

				for (unsigned int ii34 = 0; ii34 < in[u24]; ii34++) {
					if (in[u24] < du100) {
						u34 = edge2[u24][ii34];
					}
					else {
						u34 = edge2v[u24][ii34];
					}

					if ((u34 <= q4) || (vis[u34] == true) || (u34 == u14) || (u34 == u24))continue;

					if (in[u24] < du100) {
						c54 = edge2m[u24][ii34];
					}
					else {
						c54 = edge2vm[u24][ii34];
					}
					if ((b5c64 < c54) || (c64 > 3 * c54)) continue;

					mark3333[u34] = true;

					hu2222[k224][0] = u14;
					hu2222[k224][1] = u24;
					hu2222[k224][2] = u34;
					hu2222[k224][3] = c74;
					hu2222[k224][4] = c64;
					hu2222[k224][5] = c54;

					k224++;

				}
			}
		}
		for (int cz4 = 0; cz4 < k224; cz4++) {
			for (int tiz4 = cz4 + 1; tiz4 < k224; tiz4++) {
				if (hu2222[cz4][2] == hu2222[tiz4][2]) {
					if (hu2222[cz4][1] > hu2222[tiz4][1]) swap(hu2222[cz4], hu2222[tiz4]);
					if (hu2222[cz4][1] == hu2222[tiz4][1]) {
						if (hu2222[cz4][0] > hu2222[tiz4][0]) swap(hu2222[cz4], hu2222[tiz4]);
					}
				}
			}
		}
		//排序后3

		for (unsigned int i4 = 0; i4 < out[q4]; i4++) {
			if (out[q4] < du100) {
				v14 = edge1[q4][i4];
			}
			else {
				v14 = edge1v[q4][i4];
			}

			if ((v14 <= q4) || (vis[v14] == true))continue;
			if (out[q4] < du100) {
				c14 = edge1m[q4][i4];
			}
			else {
				c14 = edge1vm[q4][i4];
			}
			b5c14 = 5 * c14;
			b3c14 = 3 * c14;

			for (unsigned int j4 = 0; j4 < out[v14]; j4++) {
				if (out[v14] < du100) {
					v24 = edge1[v14][j4];
				}
				else {
					v24 = edge1v[v14][j4];
				}
				if ((v24 <= q4) || (vis[v24] == true) || (v14 == v24))continue;

				if (out[v14] < du100) {
					c24 = edge1m[v14][j4];
				}
				else {
					c24 = edge1vm[v14][j4];
				}
				b5c24 = 5 * c24;
				if ((b5c24 < c14) || (c24 > b3c14))continue;
				b3c24 = 3 * c24;

				if (mark3333[v24]) {
					for (unsigned int j54 = 0; j54 < k224; j54++) {
						if (v24 == hu2222[j54][2] && v14 != hu2222[j54][1] && v14 != hu2222[j54][0]) {
							c54 = hu2222[j54][5];
							c74 = hu2222[j54][3];
							if ((b5c14 >= c74) && (c14 <= 3 * c74) && (5 * c54 >= c24) && (c54 <= b3c24)) {
								sch5555 += hui[q4] + hui[v14] + hui[v24] + hui[hu2222[j54][1]] + hui2[hu2222[j54][0]];

								line54++;
							}
						}
					}
				}

				for (unsigned int j34 = 0; j34 < out[v24]; j34++) {
					if (out[v24] < du100) {
						v34 = edge1[v24][j34];
					}
					else {
						v34 = edge1v[v24][j34];
					}
					if (v34 < q4 || v14 == v34 || (vis[v34] == true) || (v34 == v24))continue;
					if (out[v24] < du100) {
						c34 = edge1m[v24][j34];
					}
					else {
						c34 = edge1vm[v24][j34];
					}

					b3c24 = 3 * c24;
					b5c34 = 5 * c34;
					if ((b5c34 < c24) || (c34 > b3c24))continue;

					b5c14 = 5 * c14;
					b3c34 = 3 * c34;
					if (v34 == q4) {

						if ((b5c14 >= c34) && (c14 <= b3c34)) {
							sch3333 += hui[q4] + hui[v14] + hui2[v24];

							line34++;
						}
						continue;
					}

					if (mark3333[v34]) {
						for (unsigned int j64 = 0; j64 < k224; j64++) {
							if (v34 == hu2222[j64][2] && v14 != hu2222[j64][1] && v14 != hu2222[j64][0] && v24 != hu2222[j64][1] && v24 != hu2222[j64][0]) {
								c54 = hu2222[j64][5];
								c74 = hu2222[j64][3];
								if ((b5c14 >= c74) && (c14 <= 3 * c74) && (5 * c54 >= c34) && (c54 <= b3c34)) {
									sch6666 += hui[q4] + hui[v14] + hui[v24] + hui[v34] + hui[hu2222[j64][1]] + hui2[hu2222[j64][0]];

									line64++;
								}
							}
						}
					}

					for (unsigned int j44 = 0; j44 < out[v34]; j44++) {

						if (out[v34] < du100) {
							v44 = edge1[v34][j44];
						}
						else {
							v44 = edge1v[v34][j44];
						}

						if ((v44 < q4) || v14 == v44 || v24 == v44 || (vis[v44] == true) || (v44 == v34))continue;
						if (out[v34] < du100) {
							c44 = edge1m[v34][j44];
						}
						else {
							c44 = edge1vm[v34][j44];
						}

						b3c34 = 3 * c34;
						b5c44 = 5 * c44;

						if ((b5c44 < c34) || (c44 > b3c34))continue;
						b5c14 = 5 * c14;
						b3c44 = 3 * c44;

						if (v44 == q4) {

							if ((b5c14 >= c44) && (c14 <= b3c44)) {
								sch4444 += hui[q4] + hui[v14] + hui[v24] + hui2[v34];

								line44++;
								continue;
							}
							continue;

						}

						if (mark3333[v44] == false) continue;

						if (mark3333[v44]) {
							for (unsigned int j74 = 0; j74 < k224; j74++) {
								if (v44 == hu2222[j74][2] && v14 != hu2222[j74][1] && v14 != hu2222[j74][0] && v24 != hu2222[j74][1] && v24 != hu2222[j74][0] && v34 != hu2222[j74][1] && v34 != hu2222[j74][0]) {
									c54 = hu2222[j74][5];
									c74 = hu2222[j74][3];
									if ((b5c14 >= c74) && (c14 <= 3 * c74) && (5 * c54 >= c44) && (c54 <= b3c44)) {
										sch7777 += hui[q4] + hui[v14] + hui[v24] + hui[v34] + hui[v44] + hui[hu2222[j74][1]] + hui2[hu2222[j74][0]];

										line74++;
									}
								}
							}
						}
					}
				}
			}
		}

		for (int im4 = 0; im4 < k224; im4++) {
			mark3333[hu2222[im4][2]] = false;
		}
		q4++;
	}
}

void findcycle5(int q5, int end5) {

	unsigned int v15 = 0, v25 = 0, v35 = 0, v45 = 0, v55 = 0, u15 = 0, u25 = 0, u35 = 0;
	ull c15 = 0, c25 = 0, c35 = 0, c45 = 0, c55 = 0, c65 = 0, c75 = 0;
	ull b3c15 = 0, b3c25 = 0, b3c35 = 0, b3c45 = 0, b3c55 = 0, b3c65 = 0, b3c75 = 0;
	ull b5c15 = 0, b5c25 = 0, b5c35 = 0, b5c45 = 0, b5c55 = 0, b5c65 = 0, b5c75 = 0;

	while (q5 < end5) {
		if (vis[q5]) { q5++; continue; }

		unsigned int  k225 = 0;

		for (unsigned int ii15 = 0; ii15 < in[q5]; ii15++) {
			if (in[q5] < du100) {
				u15 = edge2[q5][ii15];
			}
			else {
				u15 = edge2v[q5][ii15];
			}

			if ((u15 <= q5) || (vis[u15]))continue;

			if (in[q5] < du100) {
				c75 = edge2m[q5][ii15];
			}
			else {
				c75 = edge2vm[q5][ii15];
			}

			b5c75 = 5 * c75;

			for (unsigned int ii25 = 0; ii25 < in[u15]; ii25++) {
				if (in[u15] < du100) {
					u25 = edge2[u15][ii25];
				}
				else {
					u25 = edge2v[u15][ii25];
				}

				if ((u25 <= q5) || (vis[u25] == true) || (u25 == u15))continue;

				if (in[u15] < du100) {
					c65 = edge2m[u15][ii25];
				}
				else {
					c65 = edge2vm[u15][ii25];
				}
				if ((b5c75 < c65) || (c75 > 3 * c65)) continue;

				b5c65 = 5 * c65;

				for (unsigned int ii35 = 0; ii35 < in[u25]; ii35++) {
					if (in[u25] < du100) {
						u35 = edge2[u25][ii35];
					}
					else {
						u35 = edge2v[u25][ii35];
					}

					if ((u35 <= q5) || (vis[u35] == true) || (u35 == u15) || (u35 == u25))continue;

					if (in[u25] < du100) {
						c55 = edge2m[u25][ii35];
					}
					else {
						c55 = edge2vm[u25][ii35];
					}
					if ((b5c65 < c55) || (c65 > 3 * c55)) continue;

					mark33333[u35] = true;

					hu22222[k225][0] = u15;
					hu22222[k225][1] = u25;
					hu22222[k225][2] = u35;
					hu22222[k225][3] = c75;
					hu22222[k225][4] = c65;
					hu22222[k225][5] = c55;

					k225++;

				}
			}
		}
		for (int cz5 = 0; cz5 < k225; cz5++) {
			for (int tiz5 = cz5 + 1; tiz5 < k225; tiz5++) {
				if (hu22222[cz5][2] == hu22222[tiz5][2]) {
					if (hu22222[cz5][1] > hu22222[tiz5][1]) swap(hu22222[cz5], hu22222[tiz5]);
					if (hu22222[cz5][1] == hu22222[tiz5][1]) {
						if (hu22222[cz5][0] > hu22222[tiz5][0]) swap(hu22222[cz5], hu22222[tiz5]);
					}
				}
			}
		}
		//排序后3

		for (unsigned int i5 = 0; i5 < out[q5]; i5++) {
			if (out[q5] < du100) {
				v15 = edge1[q5][i5];
			}
			else {
				v15 = edge1v[q5][i5];
			}

			if ((v15 <= q5) || (vis[v15] == true))continue;
			if (out[q5] < du100) {
				c15 = edge1m[q5][i5];
			}
			else {
				c15 = edge1vm[q5][i5];
			}
			b5c15 = 5 * c15;
			b3c15 = 3 * c15;

			for (unsigned int j5 = 0; j5 < out[v15]; j5++) {
				if (out[v15] < du100) {
					v25 = edge1[v15][j5];
				}
				else {
					v25 = edge1v[v15][j5];
				}
				if ((v25 <= q5) || (vis[v25] == true) || (v15 == v25))continue;

				if (out[v15] < du100) {
					c25 = edge1m[v15][j5];
				}
				else {
					c25 = edge1vm[v15][j5];
				}
				b5c25 = 5 * c25;
				if ((b5c25 < c15) || (c25 > b3c15))continue;
				b3c25 = 3 * c25;

				if (mark33333[v25]) {
					for (unsigned int j55 = 0; j55 < k225; j55++) {
						if (v25 == hu22222[j55][2] && v15 != hu22222[j55][1] && v15 != hu22222[j55][0]) {
							c55 = hu22222[j55][5];
							c75 = hu22222[j55][3];
							if ((b5c15 >= c75) && (c15 <= 3 * c75) && (5 * c55 >= c25) && (c55 <= b3c25)) {
								sch55555 += hui[q5] + hui[v15] + hui[v25] + hui[hu22222[j55][1]] + hui2[hu22222[j55][0]];

								line55++;
							}
						}
					}
				}

				for (unsigned int j35 = 0; j35 < out[v25]; j35++) {
					if (out[v25] < du100) {
						v35 = edge1[v25][j35];
					}
					else {
						v35 = edge1v[v25][j35];
					}
					if (v35 < q5 || v15 == v35 || (vis[v35] == true) || (v35 == v25))continue;
					if (out[v25] < du100) {
						c35 = edge1m[v25][j35];
					}
					else {
						c35 = edge1vm[v25][j35];
					}

					b3c25 = 3 * c25;
					b5c35 = 5 * c35;
					if ((b5c35 < c25) || (c35 > b3c25))continue;

					b5c15 = 5 * c15;
					b3c35 = 3 * c35;
					if (v35 == q5) {

						if ((b5c15 >= c35) && (c15 <= b3c35)) {
							sch33333 += hui[q5] + hui[v15] + hui2[v25];

							line35++;
						}
						continue;
					}

					if (mark33333[v35]) {
						for (unsigned int j65 = 0; j65 < k225; j65++) {
							if (v35 == hu22222[j65][2] && v15 != hu22222[j65][1] && v15 != hu22222[j65][0] && v25 != hu22222[j65][1] && v25 != hu22222[j65][0]) {
								c55 = hu22222[j65][5];
								c75 = hu22222[j65][3];
								if ((b5c15 >= c75) && (c15 <= 3 * c75) && (5 * c55 >= c35) && (c55 <= b3c35)) {
									sch66666 += hui[q5] + hui[v15] + hui[v25] + hui[v35] + hui[hu22222[j65][1]] + hui2[hu22222[j65][0]];

									line65++;
								}
							}
						}
					}

					for (unsigned int j45 = 0; j45 < out[v35]; j45++) {

						if (out[v35] < du100) {
							v45 = edge1[v35][j45];
						}
						else {
							v45 = edge1v[v35][j45];
						}

						if ((v45 < q5) || v15 == v45 || v25 == v45 || (vis[v45] == true) || (v45 == v35))continue;
						if (out[v35] < du100) {
							c45 = edge1m[v35][j45];
						}
						else {
							c45 = edge1vm[v35][j45];
						}

						b3c35 = 3 * c35;
						b5c45 = 5 * c45;

						if ((b5c45 < c35) || (c45 > b3c35))continue;
						b5c15 = 5 * c15;
						b3c45 = 3 * c45;

						if (v45 == q5) {

							if ((b5c15 >= c45) && (c15 <= b3c45)) {
								sch44444 += hui[q5] + hui[v15] + hui[v25] + hui2[v35];

								line45++;
								continue;
							}
							continue;

						}

						if (mark33333[v45] == false) continue;

						if (mark33333[v45]) {
							for (unsigned int j75 = 0; j75 < k225; j75++) {
								if (v45 == hu22222[j75][2] && v15 != hu22222[j75][1] && v15 != hu22222[j75][0] && v25 != hu22222[j75][1] && v25 != hu22222[j75][0] && v35 != hu22222[j75][1] && v35 != hu22222[j75][0]) {
									c55 = hu22222[j75][5];
									c75 = hu22222[j75][3];
									if ((b5c15 >= c75) && (c15 <= 3 * c75) && (5 * c55 >= c45) && (c55 <= b3c45)) {
										sch77777 += hui[q5] + hui[v15] + hui[v25] + hui[v35] + hui[v45] + hui[hu22222[j75][1]] + hui2[hu22222[j75][0]];

										line75++;
									}
								}
							}
						}
					}
				}
			}
		}

		for (int im5 = 0; im5 < k225; im5++) {
			mark33333[hu22222[im5][2]] = false;
		}
		q5++;
	}
}

void findcycle6(int q6, int end6) {

	unsigned int v16 = 0, v26 = 0, v36 = 0, v46 = 0, v56 = 0, u16 = 0, u26 = 0, u36 = 0;
	ull c16 = 0, c26 = 0, c36 = 0, c46 = 0, c56 = 0, c66 = 0, c76 = 0;
	ull b3c16 = 0, b3c26 = 0, b3c36 = 0, b3c46 = 0, b3c56 = 0, b3c66 = 0, b3c76 = 0;
	ull b5c16 = 0, b5c26 = 0, b5c36 = 0, b5c46 = 0, b5c56 = 0, b5c66 = 0, b5c76 = 0;

	while (q6 < end6) {
		if (vis[q6]) { q6++; continue; }

		unsigned int  k226 = 0;

		for (unsigned int ii16 = 0; ii16 < in[q6]; ii16++) {
			if (in[q6] < du100) {
				u16 = edge2[q6][ii16];
			}
			else {
				u16 = edge2v[q6][ii16];
			}

			if ((u16 <= q6) || (vis[u16]))continue;

			if (in[q6] < du100) {
				c76 = edge2m[q6][ii16];
			}
			else {
				c76 = edge2vm[q6][ii16];
			}

			b5c76 = 5 * c76;

			for (unsigned int ii26 = 0; ii26 < in[u16]; ii26++) {
				if (in[u16] < du100) {
					u26 = edge2[u16][ii26];
				}
				else {
					u26 = edge2v[u16][ii26];
				}

				if ((u26 <= q6) || (vis[u26] == true) || (u26 == u16))continue;

				if (in[u16] < du100) {
					c66 = edge2m[u16][ii26];
				}
				else {
					c66 = edge2vm[u16][ii26];
				}
				if ((b5c76 < c66) || (c76 > 3 * c66)) continue;

				b5c66 = 5 * c66;

				for (unsigned int ii36 = 0; ii36 < in[u26]; ii36++) {
					if (in[u26] < du100) {
						u36 = edge2[u26][ii36];
					}
					else {
						u36 = edge2v[u26][ii36];
					}

					if ((u36 <= q6) || (vis[u36] == true) || (u36 == u16) || (u36 == u26))continue;

					if (in[u26] < du100) {
						c56 = edge2m[u26][ii36];
					}
					else {
						c56 = edge2vm[u26][ii36];
					}
					if ((b5c66 < c56) || (c66 > 3 * c56)) continue;

					mark333333[u36] = true;

					hu222222[k226][0] = u16;
					hu222222[k226][1] = u26;
					hu222222[k226][2] = u36;
					hu222222[k226][3] = c76;
					hu222222[k226][4] = c66;
					hu222222[k226][5] = c56;

					k226++;

				}
			}
		}
		for (int cz6 = 0; cz6 < k226; cz6++) {
			for (int tiz6 = cz6 + 1; tiz6 < k226; tiz6++) {
				if (hu222222[cz6][2] == hu222222[tiz6][2]) {
					if (hu222222[cz6][1] > hu222222[tiz6][1]) swap(hu222222[cz6], hu222222[tiz6]);
					if (hu222222[cz6][1] == hu222222[tiz6][1]) {
						if (hu222222[cz6][0] > hu222222[tiz6][0]) swap(hu222222[cz6], hu222222[tiz6]);
					}
				}
			}
		}
		//排序后3

		for (unsigned int i6 = 0; i6 < out[q6]; i6++) {
			if (out[q6] < du100) {
				v16 = edge1[q6][i6];
			}
			else {
				v16 = edge1v[q6][i6];
			}

			if ((v16 <= q6) || (vis[v16] == true))continue;
			if (out[q6] < du100) {
				c16 = edge1m[q6][i6];
			}
			else {
				c16 = edge1vm[q6][i6];
			}
			b5c16 = 5 * c16;
			b3c16 = 3 * c16;

			for (unsigned int j6 = 0; j6 < out[v16]; j6++) {
				if (out[v16] < du100) {
					v26 = edge1[v16][j6];
				}
				else {
					v26 = edge1v[v16][j6];
				}
				if ((v26 <= q6) || (vis[v26] == true) || (v16 == v26))continue;

				if (out[v16] < du100) {
					c26 = edge1m[v16][j6];
				}
				else {
					c26 = edge1vm[v16][j6];
				}
				b5c26 = 5 * c26;
				if ((b5c26 < c16) || (c26 > b3c16))continue;
				b3c26 = 3 * c26;

				if (mark333333[v26]) {
					for (unsigned int j56 = 0; j56 < k226; j56++) {
						if (v26 == hu222222[j56][2] && v16 != hu222222[j56][1] && v16 != hu222222[j56][0]) {
							c56 = hu222222[j56][5];
							c76 = hu222222[j56][3];
							if ((b5c16 >= c76) && (c16 <= 3 * c76) && (5 * c56 >= c26) && (c56 <= b3c26)) {
								sch555555 += hui[q6] + hui[v16] + hui[v26] + hui[hu222222[j56][1]] + hui2[hu222222[j56][0]];

								line56++;
							}
						}
					}
				}

				for (unsigned int j36 = 0; j36 < out[v26]; j36++) {
					if (out[v26] < du100) {
						v36 = edge1[v26][j36];
					}
					else {
						v36 = edge1v[v26][j36];
					}
					if (v36 < q6 || v16 == v36 || (vis[v36] == true) || (v36 == v26))continue;
					if (out[v26] < du100) {
						c36 = edge1m[v26][j36];
					}
					else {
						c36 = edge1vm[v26][j36];
					}

					b3c26 = 3 * c26;
					b5c36 = 5 * c36;
					if ((b5c36 < c26) || (c36 > b3c26))continue;

					b5c16 = 5 * c16;
					b3c36 = 3 * c36;
					if (v36 == q6) {

						if ((b5c16 >= c36) && (c16 <= b3c36)) {
							sch333333 += hui[q6] + hui[v16] + hui2[v26];

							line36++;
						}
						continue;
					}

					if (mark333333[v36]) {
						for (unsigned int j66 = 0; j66 < k226; j66++) {
							if (v36 == hu222222[j66][2] && v16 != hu222222[j66][1] && v16 != hu222222[j66][0] && v26 != hu222222[j66][1] && v26 != hu222222[j66][0]) {
								c56 = hu222222[j66][5];
								c76 = hu222222[j66][3];
								if ((b5c16 >= c76) && (c16 <= 3 * c76) && (5 * c56 >= c36) && (c56 <= b3c36)) {
									sch666666 += hui[q6] + hui[v16] + hui[v26] + hui[v36] + hui[hu222222[j66][1]] + hui2[hu222222[j66][0]];

									line66++;
								}
							}
						}
					}

					for (unsigned int j46 = 0; j46 < out[v36]; j46++) {

						if (out[v36] < du100) {
							v46 = edge1[v36][j46];
						}
						else {
							v46 = edge1v[v36][j46];
						}

						if ((v46 < q6) || v16 == v46 || v26 == v46 || (vis[v46] == true) || (v46 == v36))continue;
						if (out[v36] < du100) {
							c46 = edge1m[v36][j46];
						}
						else {
							c46 = edge1vm[v36][j46];
						}

						b3c36 = 3 * c36;
						b5c46 = 5 * c46;

						if ((b5c46 < c36) || (c46 > b3c36))continue;
						b5c16 = 5 * c16;
						b3c46 = 3 * c46;

						if (v46 == q6) {

							if ((b5c16 >= c46) && (c16 <= b3c46)) {
								sch444444 += hui[q6] + hui[v16] + hui[v26] + hui2[v36];

								line46++;
								continue;
							}
							continue;

						}

						if (mark333333[v46] == false) continue;

						if (mark333333[v46]) {
							for (unsigned int j76 = 0; j76 < k226; j76++) {
								if (v46 == hu222222[j76][2] && v16 != hu222222[j76][1] && v16 != hu222222[j76][0] && v26 != hu222222[j76][1] && v26 != hu222222[j76][0] && v36 != hu222222[j76][1] && v36 != hu222222[j76][0]) {
									c56 = hu222222[j76][5];
									c76 = hu222222[j76][3];
									if ((b5c16 >= c76) && (c16 <= 3 * c76) && (5 * c56 >= c46) && (c56 <= b3c46)) {
										sch777777 += hui[q6] + hui[v16] + hui[v26] + hui[v36] + hui[v46] + hui[hu222222[j76][1]] + hui2[hu222222[j76][0]];

										line76++;
									}
								}
							}
						}
					}
				}
			}
		}

		for (int im6 = 0; im6 < k226; im6++) {
			mark333333[hu222222[im6][2]] = false;
		}
		q6++;
	}
}


void findcycle7(int q7, int end7) {

	unsigned int v17 = 0, v27 = 0, v37 = 0, v47 = 0, v57 = 0, u17 = 0, u27 = 0, u37 = 0;
	ull c17 = 0, c27 = 0, c37 = 0, c47 = 0, c57 = 0, c67 = 0, c77 = 0;
	ull b3c17 = 0, b3c27 = 0, b3c37 = 0, b3c47 = 0, b3c57 = 0, b3c67 = 0, b3c77 = 0;
	ull b5c17 = 0, b5c27 = 0, b5c37 = 0, b5c47 = 0, b5c57 = 0, b5c67 = 0, b5c77 = 0;

	while (q7 < end7) {
		if (vis[q7]) { q7++; continue; }

		unsigned int  k227 = 0;

		for (unsigned int ii17 = 0; ii17 < in[q7]; ii17++) {
			if (in[q7] < du100) {
				u17 = edge2[q7][ii17];
			}
			else {
				u17 = edge2v[q7][ii17];
			}

			if ((u17 <= q7) || (vis[u17]))continue;

			if (in[q7] < du100) {
				c77 = edge2m[q7][ii17];
			}
			else {
				c77 = edge2vm[q7][ii17];
			}

			b5c77 = 5 * c77;

			for (unsigned int ii27 = 0; ii27 < in[u17]; ii27++) {
				if (in[u17] < du100) {
					u27 = edge2[u17][ii27];
				}
				else {
					u27 = edge2v[u17][ii27];
				}

				if ((u27 <= q7) || (vis[u27] == true) || (u27 == u17))continue;

				if (in[u17] < du100) {
					c67 = edge2m[u17][ii27];
				}
				else {
					c67 = edge2vm[u17][ii27];
				}
				if ((b5c77 < c67) || (c77 > 3 * c67)) continue;

				b5c67 = 5 * c67;

				for (unsigned int ii37 = 0; ii37 < in[u27]; ii37++) {
					if (in[u27] < du100) {
						u37 = edge2[u27][ii37];
					}
					else {
						u37 = edge2v[u27][ii37];
					}

					if ((u37 <= q7) || (vis[u37] == true) || (u37 == u17) || (u37 == u27))continue;

					if (in[u27] < du100) {
						c57 = edge2m[u27][ii37];
					}
					else {
						c57 = edge2vm[u27][ii37];
					}
					if ((b5c67 < c57) || (c67 > 3 * c57)) continue;

					mark3333333[u37] = true;

					hu2222222[k227][0] = u17;
					hu2222222[k227][1] = u27;
					hu2222222[k227][2] = u37;
					hu2222222[k227][3] = c77;
					hu2222222[k227][4] = c67;
					hu2222222[k227][5] = c57;

					k227++;

				}
			}
		}
		for (int cz7 = 0; cz7 < k227; cz7++) {
			for (int tiz7 = cz7 + 1; tiz7 < k227; tiz7++) {
				if (hu2222222[cz7][2] == hu2222222[tiz7][2]) {
					if (hu2222222[cz7][1] > hu2222222[tiz7][1]) swap(hu2222222[cz7], hu2222222[tiz7]);
					if (hu2222222[cz7][1] == hu2222222[tiz7][1]) {
						if (hu2222222[cz7][0] > hu2222222[tiz7][0]) swap(hu2222222[cz7], hu2222222[tiz7]);
					}
				}
			}
		}
		//排序后3

		for (unsigned int i7 = 0; i7 < out[q7]; i7++) {
			if (out[q7] < du100) {
				v17 = edge1[q7][i7];
			}
			else {
				v17 = edge1v[q7][i7];
			}

			if ((v17 <= q7) || (vis[v17] == true))continue;
			if (out[q7] < du100) {
				c17 = edge1m[q7][i7];
			}
			else {
				c17 = edge1vm[q7][i7];
			}
			b5c17 = 5 * c17;
			b3c17 = 3 * c17;

			for (unsigned int j7 = 0; j7 < out[v17]; j7++) {
				if (out[v17] < du100) {
					v27 = edge1[v17][j7];
				}
				else {
					v27 = edge1v[v17][j7];
				}
				if ((v27 <= q7) || (vis[v27] == true) || (v17 == v27))continue;

				if (out[v17] < du100) {
					c27 = edge1m[v17][j7];
				}
				else {
					c27 = edge1vm[v17][j7];
				}
				b5c27 = 5 * c27;
				if ((b5c27 < c17) || (c27 > b3c17))continue;
				b3c27 = 3 * c27;

				if (mark3333333[v27]) {
					for (unsigned int j57 = 0; j57 < k227; j57++) {
						if (v27 == hu2222222[j57][2] && v17 != hu2222222[j57][1] && v17 != hu2222222[j57][0]) {
							c57 = hu2222222[j57][5];
							c77 = hu2222222[j57][3];
							if ((b5c17 >= c77) && (c17 <= 3 * c77) && (5 * c57 >= c27) && (c57 <= b3c27)) {
								sch5555555 += hui[q7] + hui[v17] + hui[v27] + hui[hu2222222[j57][1]] + hui2[hu2222222[j57][0]];

								line57++;
							}
						}
					}
				}

				for (unsigned int j37 = 0; j37 < out[v27]; j37++) {
					if (out[v27] < du100) {
						v37 = edge1[v27][j37];
					}
					else {
						v37 = edge1v[v27][j37];
					}
					if (v37 < q7 || v17 == v37 || (vis[v37] == true) || (v37 == v27))continue;
					if (out[v27] < du100) {
						c37 = edge1m[v27][j37];
					}
					else {
						c37 = edge1vm[v27][j37];
					}

					b3c27 = 3 * c27;
					b5c37 = 5 * c37;
					if ((b5c37 < c27) || (c37 > b3c27))continue;

					b5c17 = 5 * c17;
					b3c37 = 3 * c37;
					if (v37 == q7) {

						if ((b5c17 >= c37) && (c17 <= b3c37)) {
							sch3333333 += hui[q7] + hui[v17] + hui2[v27];

							line37++;
						}
						continue;
					}

					if (mark3333333[v37]) {
						for (unsigned int j67 = 0; j67 < k227; j67++) {
							if (v37 == hu2222222[j67][2] && v17 != hu2222222[j67][1] && v17 != hu2222222[j67][0] && v27 != hu2222222[j67][1] && v27 != hu2222222[j67][0]) {
								c57 = hu2222222[j67][5];
								c77 = hu2222222[j67][3];
								if ((b5c17 >= c77) && (c17 <= 3 * c77) && (5 * c57 >= c37) && (c57 <= b3c37)) {
									sch6666666 += hui[q7] + hui[v17] + hui[v27] + hui[v37] + hui[hu2222222[j67][1]] + hui2[hu2222222[j67][0]];

									line67++;
								}
							}
						}
					}

					for (unsigned int j47 = 0; j47 < out[v37]; j47++) {

						if (out[v37] < du100) {
							v47 = edge1[v37][j47];
						}
						else {
							v47 = edge1v[v37][j47];
						}

						if ((v47 < q7) || v17 == v47 || v27 == v47 || (vis[v47] == true) || (v47 == v37))continue;
						if (out[v37] < du100) {
							c47 = edge1m[v37][j47];
						}
						else {
							c47 = edge1vm[v37][j47];
						}

						b3c37 = 3 * c37;
						b5c47 = 5 * c47;

						if ((b5c47 < c37) || (c47 > b3c37))continue;
						b5c17 = 5 * c17;
						b3c47 = 3 * c47;

						if (v47 == q7) {

							if ((b5c17 >= c47) && (c17 <= b3c47)) {
								sch4444444 += hui[q7] + hui[v17] + hui[v27] + hui2[v37];

								line47++;
								continue;
							}
							continue;

						}

						if (mark3333333[v47] == false) continue;

						if (mark3333333[v47]) {
							for (unsigned int j77 = 0; j77 < k227; j77++) {
								if (v47 == hu2222222[j77][2] && v17 != hu2222222[j77][1] && v17 != hu2222222[j77][0] && v27 != hu2222222[j77][1] && v27 != hu2222222[j77][0] && v37 != hu2222222[j77][1] && v37 != hu2222222[j77][0]) {
									c57 = hu2222222[j77][5];
									c77 = hu2222222[j77][3];
									if ((b5c17 >= c77) && (c17 <= 3 * c77) && (5 * c57 >= c47) && (c57 <= b3c47)) {
										sch7777777 += hui[q7] + hui[v17] + hui[v27] + hui[v37] + hui[v47] + hui[hu2222222[j77][1]] + hui2[hu2222222[j77][0]];

										line77++;
									}
								}
							}
						}
					}
				}
			}
		}

		for (int im7 = 0; im7 < k227; im7++) {
			mark3333333[hu2222222[im7][2]] = false;
		}
		q7++;
	}
}


void findcycle8(int q8, int end8) {

	unsigned int v18 = 0, v28 = 0, v38 = 0, v48 = 0, v58 = 0, u18 = 0, u28 = 0, u38 = 0;
	ull c17 = 0, c27 = 0, c37 = 0, c47 = 0, c57 = 0, c67 = 0, c77 = 0;
	ull b3c17 = 0, b3c27 = 0, b3c37 = 0, b3c47 = 0, b3c57 = 0, b3c67 = 0, b3c77 = 0;
	ull b5c17 = 0, b5c27 = 0, b5c37 = 0, b5c47 = 0, b5c57 = 0, b5c67 = 0, b5c77 = 0;

	while (q8 < end8) {
		if (vis[q8]) { q8++; continue; }

		unsigned int  k228 = 0;

		for (unsigned int ii18 = 0; ii18 < in[q8]; ii18++) {
			if (in[q8] < du100) {
				u18 = edge2[q8][ii18];
			}
			else {
				u18 = edge2v[q8][ii18];
			}

			if ((u18 <= q8) || (vis[u18]))continue;

			if (in[q8] < du100) {
				c77 = edge2m[q8][ii18];
			}
			else {
				c77 = edge2vm[q8][ii18];
			}

			b5c77 = 5 * c77;

			for (unsigned int ii28 = 0; ii28 < in[u18]; ii28++) {
				if (in[u18] < du100) {
					u28 = edge2[u18][ii28];
				}
				else {
					u28 = edge2v[u18][ii28];
				}

				if ((u28 <= q8) || (vis[u28] == true) || (u28 == u18))continue;

				if (in[u18] < du100) {
					c67 = edge2m[u18][ii28];
				}
				else {
					c67 = edge2vm[u18][ii28];
				}
				if ((b5c77 < c67) || (c77 > 3 * c67)) continue;

				b5c67 = 5 * c67;

				for (unsigned int ii38 = 0; ii38 < in[u28]; ii38++) {
					if (in[u28] < du100) {
						u38 = edge2[u28][ii38];
					}
					else {
						u38 = edge2v[u28][ii38];
					}

					if ((u38 <= q8) || (vis[u38] == true) || (u38 == u18) || (u38 == u28))continue;

					if (in[u28] < du100) {
						c57 = edge2m[u28][ii38];
					}
					else {
						c57 = edge2vm[u28][ii38];
					}
					if ((b5c67 < c57) || (c67 > 3 * c57)) continue;

					mark33333333[u38] = true;

					hu22222222[k228][0] = u18;
					hu22222222[k228][1] = u28;
					hu22222222[k228][2] = u38;
					hu22222222[k228][3] = c77;
					hu22222222[k228][4] = c67;
					hu22222222[k228][5] = c57;

					k228++;

				}
			}
		}
		for (int cz8 = 0; cz8 < k228; cz8++) {
			for (int tiz8 = cz8 + 1; tiz8 < k228; tiz8++) {
				if (hu22222222[cz8][2] == hu22222222[tiz8][2]) {
					if (hu22222222[cz8][1] > hu22222222[tiz8][1]) swap(hu22222222[cz8], hu22222222[tiz8]);
					if (hu22222222[cz8][1] == hu22222222[tiz8][1]) {
						if (hu22222222[cz8][0] > hu22222222[tiz8][0]) swap(hu22222222[cz8], hu22222222[tiz8]);
					}
				}
			}
		}
		//排序后3

		for (unsigned int i8 = 0; i8 < out[q8]; i8++) {
			if (out[q8] < du100) {
				v18 = edge1[q8][i8];
			}
			else {
				v18 = edge1v[q8][i8];
			}

			if ((v18 <= q8) || (vis[v18] == true))continue;
			if (out[q8] < du100) {
				c17 = edge1m[q8][i8];
			}
			else {
				c17 = edge1vm[q8][i8];
			}
			b5c17 = 5 * c17;
			b3c17 = 3 * c17;

			for (unsigned int j8 = 0; j8 < out[v18]; j8++) {
				if (out[v18] < du100) {
					v28 = edge1[v18][j8];
				}
				else {
					v28 = edge1v[v18][j8];
				}
				if ((v28 <= q8) || (vis[v28] == true) || (v18 == v28))continue;

				if (out[v18] < du100) {
					c27 = edge1m[v18][j8];
				}
				else {
					c27 = edge1vm[v18][j8];
				}
				b5c27 = 5 * c27;
				if ((b5c27 < c17) || (c27 > b3c17))continue;
				b3c27 = 3 * c27;

				if (mark33333333[v28]) {
					for (unsigned int j58 = 0; j58 < k228; j58++) {
						if (v28 == hu22222222[j58][2] && v18 != hu22222222[j58][1] && v18 != hu22222222[j58][0]) {
							c57 = hu22222222[j58][5];
							c77 = hu22222222[j58][3];
							if ((b5c17 >= c77) && (c17 <= 3 * c77) && (5 * c57 >= c27) && (c57 <= b3c27)) {
								sch55555555 += hui[q8] + hui[v18] + hui[v28] + hui[hu22222222[j58][1]] + hui2[hu22222222[j58][0]];

								line58++;
							}
						}
					}
				}

				for (unsigned int j38 = 0; j38 < out[v28]; j38++) {
					if (out[v28] < du100) {
						v38 = edge1[v28][j38];
					}
					else {
						v38 = edge1v[v28][j38];
					}
					if (v38 < q8 || v18 == v38 || (vis[v38] == true) || (v38 == v28))continue;
					if (out[v28] < du100) {
						c37 = edge1m[v28][j38];
					}
					else {
						c37 = edge1vm[v28][j38];
					}

					b3c27 = 3 * c27;
					b5c37 = 5 * c37;
					if ((b5c37 < c27) || (c37 > b3c27))continue;

					b5c17 = 5 * c17;
					b3c37 = 3 * c37;
					if (v38 == q8) {

						if ((b5c17 >= c37) && (c17 <= b3c37)) {
							sch33333333 += hui[q8] + hui[v18] + hui2[v28];

							line38++;
						}
						continue;
					}

					if (mark33333333[v38]) {
						for (unsigned int j68 = 0; j68 < k228; j68++) {
							if (v38 == hu22222222[j68][2] && v18 != hu22222222[j68][1] && v18 != hu22222222[j68][0] && v28 != hu22222222[j68][1] && v28 != hu22222222[j68][0]) {
								c57 = hu22222222[j68][5];
								c77 = hu22222222[j68][3];
								if ((b5c17 >= c77) && (c17 <= 3 * c77) && (5 * c57 >= c37) && (c57 <= b3c37)) {
									sch66666666 += hui[q8] + hui[v18] + hui[v28] + hui[v38] + hui[hu22222222[j68][1]] + hui2[hu22222222[j68][0]];

									line68++;
								}
							}
						}
					}

					for (unsigned int j48 = 0; j48 < out[v38]; j48++) {

						if (out[v38] < du100) {
							v48 = edge1[v38][j48];
						}
						else {
							v48 = edge1v[v38][j48];
						}

						if ((v48 < q8) || v18 == v48 || v28 == v48 || (vis[v48] == true) || (v48 == v38))continue;
						if (out[v38] < du100) {
							c47 = edge1m[v38][j48];
						}
						else {
							c47 = edge1vm[v38][j48];
						}

						b3c37 = 3 * c37;
						b5c47 = 5 * c47;

						if ((b5c47 < c37) || (c47 > b3c37))continue;
						b5c17 = 5 * c17;
						b3c47 = 3 * c47;

						if (v48 == q8) {

							if ((b5c17 >= c47) && (c17 <= b3c47)) {
								sch44444444 += hui[q8] + hui[v18] + hui[v28] + hui2[v38];

								line48++;
								continue;
							}
							continue;

						}

						if (mark33333333[v48] == false) continue;

						if (mark33333333[v48]) {
							for (unsigned int j78 = 0; j78 < k228; j78++) {
								if (v48 == hu22222222[j78][2] && v18 != hu22222222[j78][1] && v18 != hu22222222[j78][0] && v28 != hu22222222[j78][1] && v28 != hu22222222[j78][0] && v38 != hu22222222[j78][1] && v38 != hu22222222[j78][0]) {
									c57 = hu22222222[j78][5];
									c77 = hu22222222[j78][3];
									if ((b5c17 >= c77) && (c17 <= 3 * c77) && (5 * c57 >= c47) && (c57 <= b3c47)) {
										sch77777777 += hui[q8] + hui[v18] + hui[v28] + hui[v38] + hui[v48] + hui[hu22222222[j78][1]] + hui2[hu22222222[j78][0]];

										line78++;
									}
								}
							}
						}
					}
				}
			}
		}

		for (int im8 = 0; im8 < k228; im8++) {
			mark33333333[hu22222222[im8][2]] = false;
		}
		q8++;
	}
}


void xie(const string& outputfile) {
	FILE* fp = fopen(outputfile.c_str(), "wb");
	char buf[1024];

	int idx = sprintf(buf, "%d\r\n", allline);
	buf[idx] = '\0';
	fwrite(buf, idx, sizeof(char), fp);

	fwrite(sch3.c_str(), sch3.size(), sizeof(char), fp);
	fwrite(sch33.c_str(), sch33.size(), sizeof(char), fp);
	fwrite(sch333.c_str(), sch333.size(), sizeof(char), fp);
	fwrite(sch3333.c_str(), sch3333.size(), sizeof(char), fp);
	fwrite(sch33333.c_str(), sch33333.size(), sizeof(char), fp);
	fwrite(sch333333.c_str(), sch333333.size(), sizeof(char), fp);
	fwrite(sch3333333.c_str(), sch3333333.size(), sizeof(char), fp);
	fwrite(sch33333333.c_str(), sch33333333.size(), sizeof(char), fp);
	fwrite(sch4.c_str(), sch4.size(), sizeof(char), fp);
	fwrite(sch44.c_str(), sch44.size(), sizeof(char), fp);
	fwrite(sch444.c_str(), sch444.size(), sizeof(char), fp);
	fwrite(sch4444.c_str(), sch4444.size(), sizeof(char), fp);
	fwrite(sch44444.c_str(), sch44444.size(), sizeof(char), fp);
	fwrite(sch444444.c_str(), sch444444.size(), sizeof(char), fp);
	fwrite(sch4444444.c_str(), sch4444444.size(), sizeof(char), fp);
	fwrite(sch44444444.c_str(), sch44444444.size(), sizeof(char), fp);
	fwrite(sch5.c_str(), sch5.size(), sizeof(char), fp);
	fwrite(sch55.c_str(), sch55.size(), sizeof(char), fp);
	fwrite(sch555.c_str(), sch555.size(), sizeof(char), fp);
	fwrite(sch5555.c_str(), sch5555.size(), sizeof(char), fp);
	fwrite(sch55555.c_str(), sch55555.size(), sizeof(char), fp);
	fwrite(sch555555.c_str(), sch555555.size(), sizeof(char), fp);
	fwrite(sch5555555.c_str(), sch5555555.size(), sizeof(char), fp);
	fwrite(sch55555555.c_str(), sch55555555.size(), sizeof(char), fp);
	fwrite(sch6.c_str(), sch6.size(), sizeof(char), fp);
	fwrite(sch66.c_str(), sch66.size(), sizeof(char), fp);
	fwrite(sch666.c_str(), sch666.size(), sizeof(char), fp);
	fwrite(sch6666.c_str(), sch6666.size(), sizeof(char), fp);
	fwrite(sch66666.c_str(), sch66666.size(), sizeof(char), fp);
	fwrite(sch666666.c_str(), sch666666.size(), sizeof(char), fp);
	fwrite(sch6666666.c_str(), sch6666666.size(), sizeof(char), fp);
	fwrite(sch66666666.c_str(), sch66666666.size(), sizeof(char), fp);
	fwrite(sch7.c_str(), sch7.size(), sizeof(char), fp);
	fwrite(sch77.c_str(), sch77.size(), sizeof(char), fp);
	fwrite(sch777.c_str(), sch777.size(), sizeof(char), fp);
	fwrite(sch7777.c_str(), sch7777.size(), sizeof(char), fp);
	fwrite(sch77777.c_str(), sch77777.size(), sizeof(char), fp);
	fwrite(sch777777.c_str(), sch777777.size(), sizeof(char), fp);
	fwrite(sch7777777.c_str(), sch7777777.size(), sizeof(char), fp);
	fwrite(sch77777777.c_str(), sch77777777.size(), sizeof(char), fp);

	//exit(0);
	fclose(fp);
}

int main()
{
	//sleep(5);
	cout << "8xcstart:" << endl;
	clock_t start, second, three;
	start = clock();

	string testFile = "/root/data/200w.txt";
	string outputfile = "/root/8xcresult.txt";
	duwenjian(testFile);
	newGraph();
	//cout <<"入度最大值"<< (*max_element(in, in + nodeCnt)) << endl;
	//cout << "出度最大值" << (*max_element(out, out + nodeCnt)) << endl;

	dian2sort();
	diansort();
	qian();
	//cout << "数据写入链表时间" << clock() - start << "ms" << endl;
	second = clock();

	//cout << "V=" << V << endl;//最大id
	//cout << "nodeCnt=" << nodeCnt << endl;//节点数
	unsigned int n1, n2, n3, n4, n5, n6, n7;
	n1 = 28 * nodeCnt / 1000;    //0.028
	n2 = 55 * nodeCnt / 1000;    //0.055
	n3 = 9 * nodeCnt / 100;      //0.09
	n4 = 13 * nodeCnt / 100;     //0.13
	n5 = 185 * nodeCnt / 1000;   //0.185
	n6 = 25 * nodeCnt / 100;     //0.25
	n7 = 348 * nodeCnt / 1000;   //0.348

	//findcycle1(0, nodeCnt);
	thread myxc1(findcycle1, 0, n1);
	thread myxc2(findcycle2, n1, n2);
	thread myxc3(findcycle3, n2, n3);
	//findcycle4(n3, n4);

	thread myxc5(findcycle5, n4, n5);
	thread myxc6(findcycle6, n5, n6);
	thread myxc7(findcycle7, n6, n7);
	thread myxc8(findcycle8, n7, nodeCnt);
	findcycle4(n3, n4);

	myxc1.join();
	myxc2.join();
	myxc3.join();
	myxc5.join();
	myxc6.join();
	myxc7.join();
	myxc8.join();

	cout << "dfs time=" << clock() - second << "ms" << endl;

	line3 = line31 + line32 + line33 + line34 + line35 + line36 + line37 + line38;
	line4 = line41 + line42 + line43 + line44 + line45 + line46 + line47 + line48;
	line5 = line51 + line52 + line53 + line54 + line55 + line56 + line57 + line58;
	line6 = line61 + line62 + line63 + line64 + line65 + line66 + line67 + line68;
	line7 = line71 + line72 + line73 + line74 + line75 + line76 + line77 + line78;

	allline = line3 + line4 + line5 + line6 + line7;

	//cout << "开始输出" << endl;
	three = clock();

	xie(outputfile);

	cout << "allline=" << allline << endl;

	cout << "line3=" << line3 << endl; cout << "line4=" << line4 << endl; cout << "line5=" << line5 << endl; cout << "line6=" << line6 << endl; cout << "line7=" << line7 << endl;





	//cout << "输出时间" << clock() - three << "ms" << endl;
	cout << "all time=" << clock() << "ms" << endl;
	//cout << "结束" << endl;

	exit(0);

	//return 0;
}




