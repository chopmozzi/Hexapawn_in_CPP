#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#pragma warning(disable:4996)
using namespace std;
//현재 상태 저장 Data
class Data
{
private:
	string color; //측정 시 기준이 되는 color
	int level;	  //현재 tree의 level
	int e_value;  //evaluation 값
	vector<vector<char>> board; //hexapawn board

public:
	Data() //constructor 초기화
	{
		color = "\0";
		level = 0;
		e_value = 0;
		for (int i = 0; i < 3; i++)
		{
			vector<char> arr(3, 0);
			board.push_back(arr);
		}

	}
	//각 private 변수에 대한 get, set 함수 작성
	void setColor(string str)
	{
		color = str;
	}
	string getColor()
	{
		return color;
	}
	void setLev(int num)
	{
		level = num;
	}
	int getLev()
	{
		return level;
	}
	void setEva(int num)
	{
		e_value = num;
	}
	int getEva()
	{
		return e_value;
	}
	void setBoard(vector<vector<char>> arr)
	{
		board = arr;
	}
	vector<vector<char>> getBoard()
	{
		return board;
	}
};
//alpha beta prunning을 위한 minimax 함수
int AlphaBetaMax(Data* pNode, int alpha, int beta); //Max 값 구하기
int AlphaBetaMin(Data* pNode, int alpha, int beta); //Min 값 구하기
//Game Tree 전역 변수 선언
vector<Data*> tree;
//가장 기본적인 Board 생성
//2차원 vector의 형태로 return
vector<vector<char>> InitBoard()
{
	vector<char> arr1(3, 'W');
	vector<char> arr2(3, 'O');
	vector<char> arr3(3, 'B');
	vector<vector<char>> brr;
	brr.push_back(arr1);
	brr.push_back(arr2);
	brr.push_back(arr3);
	return brr;
}
//2차원 vector 형태의 board를 받고
//현재 Board 상황에 맞는 value 계산 후 return
int CheckValue(vector<vector<char>> c_board)
{
	int value = 0;
	bool gameover = false;
	auto wcheck = find(c_board[2].begin(), c_board[2].end(), 'W'); //White pawn이 끝에 도달했는지 찾기 위함
	auto bcheck = find(c_board[0].begin(), c_board[0].end(), 'B'); //Black pawn이 끝에 도달했는지 찾기 위함
	int wnum = 0; //white pawn 카운트
	int bnum = 0; //black pawn 카운트
	int wvalue = 0; //white 측의 evaluation 계산
	int bvalue = 0; //black 측의 evaluation 계산
	//각 count 및 evaluation 계산
	for (int i = 0; i < 3; i++)
	{
		wnum += count(c_board[i].begin(), c_board[i].end(), 'W');
		bnum += count(c_board[i].begin(), c_board[i].end(), 'B');
		wvalue += count(c_board[i].begin(), c_board[i].end(), 'W') * (i + 1) * (i + 1);
		bvalue += count(c_board[i].begin(), c_board[i].end(), 'B') * (3 - i) * (3 - i);

	}
	//white pawn이 끝에 도달했을 경우(white 승리)
	if (wcheck != c_board[2].end())
	{
		return 100;
	}
	//black pawn이 끝에 도달했을 경우(black 승리)
	if (bcheck != c_board[0].end())
	{
		return -100;
	}
	//white pawn이 다 죽었을 경우(white 패배)
	if (wnum == 0)
	{
		return -100;
	}
	//black pawn이 다 죽었을 경우(black 패배)
	if (bnum == 0)
	{
		return 100;
	}
	//더 이상 움직일 수 없는 경우 판별
	if (wnum == 1 && bnum == 2)
	{
		//white pawn이 못 움직이는 경우(white 패배)
		if (c_board[1][0] == 'W' && c_board[1][1] == 'B' && c_board[2][1] == 'B')
			return -100;
		if (c_board[1][1] == 'B' && c_board[1][2] == 'W' && c_board[2][2] == 'B')
			return -100;
	}
	if (wnum == 1 && bnum == 3)
	{	//white pawn이 못 움직이는 경우(white 패배)
		if (c_board[1][0] == 'B' && c_board[1][1] == 'W' && c_board[1][2] == 'B' && c_board[2][1] == 'B')
			return -100;
	}
	if (wnum == 2 && bnum == 1)
	{	//black pawn이 못 움직이는 경우(black 패배)
		if (c_board[0][0] == 'W' && c_board[1][0] == 'B' && c_board[1][1] == 'W')
			return 100;
		if (c_board[0][2] == 'W' && c_board[1][1] == 'W' && c_board[1][2] == 'B')
			return 100;
	}
	if (wnum == 3 && bnum == 1)
	{	//balck pawn이 못 움직이는 경우(black 패배)
		if (c_board[0][1] == 'W' && c_board[1][0] == 'W' && c_board[1][1] == 'B' && c_board[1][2] == 'W')
			return 100;
	}
	//evaluation value를 반환
	return wvalue - bvalue;
}
//tree의 Data를 받고 Data의 Board를 출력
void ShowBoard(Data* pD)
{
	cout << "=================" << endl;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (pD->getBoard()[i][j] == 'O')
			{
				cout << "|   |";
			}
			else
			{
				cout << "| " << pD->getBoard()[i][j] << " |";
			}
		}
		cout << endl;
	}
	cout << "=================" << endl;
}
//pawn의 color와 board 상태를 받고 해당 x,y좌표를 값으로 받음
vector<pair<int, int>> getlocation(vector<vector<char>> board, char color)
{	//x,y좌표 값을 가지는 vector
	vector<pair<int, int>> xylocation;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (board[i][j] == color)
			{
				xylocation.push_back(make_pair(i, j));
			}
		}
	}
	return xylocation;
}
//AlphaBeta prunning을 위해 Max값을 계산하는 function
//alpha값과 beta값을 받고 Data를 통해 evaluation value를 계산하여 alpha 및 beta 반환
//Recursive하게 동작
//Max는 White 위치에서 계산
int AlphaBetaMax(Data* pNode, int alpha, int beta)
{
	int temp;
	int value = CheckValue(pNode->getBoard()); //evaluation 계산
	//승리 및 패배가 결정 났을 때
	if (value == 100 || value == -100)
	{
		return value;
	}
	//3 ply에 도달 했을 때
	if (pNode->getLev() == 3)
	{
		return value;
	}
	vector<vector<char>> cur_b; //current board
	vector<vector<char>> nex_b; //next board
	vector<pair<int, int>> comloc; //computer가 play하는 측의 pawn x,y좌표 계산
	vector<pair<int, int>> useloc; //player가 play하는 측의 pawn x,y좌표 계산
	char ccolor = pNode->getColor()[0]; //computer color 받음
	char ucolor; //user color의 반대
	if (ccolor == 'B')
	{
		ucolor = 'W';
	}
	else
	{
		ucolor = 'B';
	}
	//current board 받기
	cur_b = pNode->getBoard();
	//각 x, y좌표 위치 받기
	comloc = getlocation(cur_b, ccolor);
	useloc = getlocation(cur_b, ucolor);
	//computer location을 받고 각 state에 대한 evaluation value 계산
	for (int i = 0; i < comloc.size(); i++)
	{	//computer location에 대한 user location 위치 찾기
		auto it = find(useloc.begin(), useloc.end(), make_pair(comloc[i].first + 1, comloc[i].second - 1));
		//왼쪽 대각선
		if (comloc[i].second - 1 >= 0 && comloc[i].first + 1 < 3)
		{	//왼쪽 대각선에 computer에 반대되는 색깔이 있을 경우
			if (it != useloc.end())
			{	//board update
				nex_b = cur_b;
				nex_b[comloc[i].first][comloc[i].second] = 'O';
				nex_b[comloc[i].first + 1][comloc[i].second - 1] = ccolor;
				//다음 트리 노드로 갱신
				Data* pNew = new Data;
				pNew->setBoard(nex_b);
				pNew->setLev(pNode->getLev() + 1);
				pNew->setColor("Black");
				temp = AlphaBetaMin(pNew, alpha, beta);
				//value가 alpha보다 크면 alpha값을 value로 갱신
				if (temp > alpha)
				{
					alpha = temp;
					//level 1에서 부터의 state 값 반환
					if (pNew->getLev() == 1)
					{	//alpha beta prunning
						tree[1] = pNew;
					}
				}
			}
		}
		//오른쪽 대각선에 computer에 반대되는 색깔이 있을 경우
		it = find(useloc.begin(), useloc.end(), make_pair(comloc[i].first + 1, comloc[i].second + 1));
		if (comloc[i].second + 1 < 3 && comloc[i].first + 1 < 3)
		{
			if (it != useloc.end())
			{	//board 갱신
				nex_b = cur_b;
				nex_b[comloc[i].first][comloc[i].second] = 'O';
				nex_b[comloc[i].first + 1][comloc[i].second + 1] = ccolor;
				Data* pNew = new Data;
				pNew->setBoard(nex_b);
				pNew->setLev(pNode->getLev() + 1);
				pNew->setColor("Black");
				temp = AlphaBetaMin(pNew, alpha, beta);
				if (temp > alpha)
				{
					alpha = temp;
					if (pNew->getLev() == 1)
					{
						tree[1] = pNew;
					}
				}
			}
		}
		//앞으로 이동 가능할 때
		if (comloc[i].first + 1 < 3)
		{
			if (cur_b[comloc[i].first + 1][comloc[i].second] == 'O')
			{
				nex_b = cur_b;
				nex_b[comloc[i].first][comloc[i].second] = 'O';
				nex_b[comloc[i].first + 1][comloc[i].second] = ccolor;
				Data* pNew = new Data;
				pNew->setBoard(nex_b);
				pNew->setLev(pNode->getLev() + 1);
				pNew->setColor("Black");
				temp = AlphaBetaMin(pNew, alpha, beta);
				if (temp > alpha)
				{
					alpha = temp;
					if (pNew->getLev() == 1)
					{
						tree[1] = pNew;
					}
				}
			}
		}
		//왼쪽으로 이동
		if (comloc[i].second - 1 >= 0)
		{
			if (cur_b[comloc[i].first][comloc[i].second - 1] == 'O')
			{
				nex_b = cur_b;
				nex_b[comloc[i].first][comloc[i].second] = 'O';
				nex_b[comloc[i].first][comloc[i].second - 1] = ccolor;
				Data* pNew = new Data;
				pNew->setBoard(nex_b);
				pNew->setLev(pNode->getLev() + 1);
				pNew->setColor("Black");
				temp = AlphaBetaMin(pNew, alpha, beta);
				if (temp > alpha)
				{
					alpha = temp;
					if (pNew->getLev() == 1)
					{
						tree[1] = pNew;
					}
				}
			}
		}
		//오른쪽으로 이동
		if (comloc[i].second + 1 < 3)
		{
			if (cur_b[comloc[i].first][comloc[i].second + 1] == 'O')
			{
				nex_b = cur_b;
				nex_b[comloc[i].first][comloc[i].second] = 'O';
				nex_b[comloc[i].first][comloc[i].second + 1] = ccolor;
				Data* pNew = new Data;
				pNew->setBoard(nex_b);
				pNew->setLev(pNode->getLev() + 1);
				pNew->setColor("Black");
				temp = AlphaBetaMin(pNew, alpha, beta);
				if (temp > alpha)
				{
					alpha = temp;
					if (pNew->getLev() == 1)
					{
						tree[1] = pNew;
					}
				}
			}
		}
		//alpha값과 beta를 비교 하고 beta 반환
		if (alpha >= beta)
		{
			return beta;
		}

	}
	//alpha 반환
	return alpha;
}
//Alpha Beta prunning을 위한 Min 값 구하기
//alpha와 beta값을 받고 현재 Data에 대한 value 계산
//Black위치에서 value 계산
//Max MIn을 왔다갔다하며 recursive call
int AlphaBetaMin(Data* pNode, int alpha, int beta)
{
	int temp;
	//현재 board에 대한 evaluation value 계산
	int value = CheckValue(pNode->getBoard());
	//승리 및 패배가 결정 났을 때
	if (value == 100 || value == -100)
	{
		return value;
	}
	//3 ply에 도달했을 때 value 반환
	if (pNode->getLev() == 3)
	{
		return value;
	}

	vector<vector<char>> cur_b; //current board
	vector<vector<char>> nex_b; //next board
	vector<pair<int, int>> comloc; //computer pawn location
	vector<pair<int, int>> useloc; //user pawn location
	char ccolor = pNode->getColor()[0]; //computer color
	char ucolor; //user color
	if (ccolor == 'B')
	{
		ucolor = 'W';
	}
	else
	{
		ucolor = 'B';
	}
	//load current board
	cur_b = pNode->getBoard();
	//load computer location
	comloc = getlocation(cur_b, ccolor);
	//load user location
	useloc = getlocation(cur_b, ucolor);
	//computer loaction에 따른 state value 계산(Black)
	for (int i = 0; i < comloc.size(); i++)
	{	//왼쪽 대각선 계산
		auto it = find(useloc.begin(), useloc.end(), make_pair(comloc[i].first - 1, comloc[i].second - 1));
		if (comloc[i].second - 1 >= 0 && comloc[i].first - 1 >= 0)
		{
			if (it != useloc.end())
			{
				nex_b = cur_b;
				nex_b[comloc[i].first][comloc[i].second] = 'O';
				nex_b[comloc[i].first - 1][comloc[i].second - 1] = ccolor;
				Data* pNew = new Data;
				pNew->setBoard(nex_b);
				pNew->setLev(pNode->getLev() + 1);
				pNew->setColor("White");
				temp = AlphaBetaMax(pNew, alpha, beta); //Call Max function
				if (temp < beta)
				{
					beta = temp;
					if (pNew->getLev() == 1)
					{
						tree[1] = pNew;
					}
				}
			}
		}
		it = find(useloc.begin(), useloc.end(), make_pair(comloc[i].first - 1, comloc[i].second + 1));
		//오른쪽 대각선 계산
		if (comloc[i].second + 1 < 3 && comloc[i].first - 1 > 0)
		{
			if (it != useloc.end())
			{
				nex_b = cur_b;
				nex_b[comloc[i].first][comloc[i].second] = 'O';
				nex_b[comloc[i].first - 1][comloc[i].second + 1] = ccolor;
				Data* pNew = new Data;
				pNew->setBoard(nex_b);
				pNew->setLev(pNode->getLev() + 1);
				pNew->setColor("White");
				temp = AlphaBetaMax(pNew, alpha, beta);
				if (temp < beta)
				{
					beta = temp;
					if (pNew->getLev() == 1)
					{
						tree[1] = pNew;
					}
				}
			}
		}
		//앞으로 이동
		if (comloc[i].first - 1 >= 0)
		{
			if (cur_b[comloc[i].first - 1][comloc[i].second] == 'O')
			{
				nex_b = cur_b;
				nex_b[comloc[i].first][comloc[i].second] = 'O';
				nex_b[comloc[i].first - 1][comloc[i].second] = ccolor;
				Data* pNew = new Data;
				//pNew->setBoard(cur_b);
				pNew->setBoard(nex_b);
				pNew->setLev(pNode->getLev() + 1);
				pNew->setColor("White");
				temp = AlphaBetaMax(pNew, alpha, beta);
				if (temp < beta)
				{
					beta = temp;
					if (pNew->getLev() == 1)
					{
						tree[1] = pNew;
					}
				}
			}
		}
		//왼쪽 이동
		if (comloc[i].second - 1 >= 0)
		{
			if (cur_b[comloc[i].first][comloc[i].second - 1] == 'O')
			{
				nex_b = cur_b;
				nex_b[comloc[i].first][comloc[i].second] = 'O';
				nex_b[comloc[i].first][comloc[i].second - 1] = ccolor;
				Data* pNew = new Data;
				//pNew->setBoard(cur_b);
				pNew->setBoard(nex_b);
				pNew->setLev(pNode->getLev() + 1);
				pNew->setColor("White");
				temp = AlphaBetaMax(pNew, alpha, beta);
				if (temp < beta)
				{
					beta = temp;
					if (pNew->getLev() == 1)
					{
						tree[1] = pNew;
					}
				}
			}
		}
		//오른쪽 이동
		if (comloc[i].second + 1 < 3)
		{
			if (cur_b[comloc[i].first][comloc[i].second + 1] == 'O')
			{
				nex_b = cur_b;
				nex_b[comloc[i].first][comloc[i].second] = 'O';
				nex_b[comloc[i].first][comloc[i].second + 1] = ccolor;
				Data* pNew = new Data;
				//pNew->setBoard(cur_b);
				pNew->setBoard(nex_b);
				pNew->setLev(pNode->getLev() + 1);
				pNew->setColor("White");
				temp = AlphaBetaMax(pNew, alpha, beta);
				if (temp < beta)
				{
					beta = temp;
					if (pNew->getLev() == 1)
					{
						tree[1] = pNew;
					}
				}
			}
		}
		//alpha가 beta보다 크면
		if (alpha >= beta)
		{	//alpha return
			return alpha;
		}

	}
	return beta;
}
//현재 위치와 움직일 위치 받아서 움직일 수 있는지 확인
bool CheckPawn(vector<vector<char>> board, int x, int y, int mx, int my, char ucolor)
{	//범위를 넘는지 확인
	if (mx > 3 || my > 3 || mx < 1 || my < 1)
	{
		return false;
	}
	if (my - y > 1 || y - my > 1)
	{
		return false;
	}
	//각 위치에 대한 비교
	if (ucolor == 'W')
	{
		if (mx - x > 1)
		{
			return false;
		}
		if (board[mx - 1][my - 1] == 'O' || board[mx - 1][my - 1] == 'B')
		{
			if (x > mx)
			{
				return false;
			}
			if (mx - x == 0 && board[mx - 1][my - 1] == 'B')
			{
				return false;
			}
			if (mx - x == 1 && my - y == 0 && board[mx - 1][my - 1] == 'B')
			{
				return false;
			}
			if (mx - x == 1 && (my - y == 1 || my - y == -1) && board[mx - 1][my - 1] == 'O')
			{
				return false;
			}
			return true;
		}
		return false;
	}
	else
	{
		if (x - mx > 1)
		{
			return false;
		}
		if (board[mx - 1][my - 1] == 'O' || board[mx - 1][my - 1] == 'W')
		{

			if (x < mx)
			{
				return false;
			}
			if (x - mx == 1 && my - y == 0 && board[mx - 1][my - 1] == 'W')
			{
				return false;
			}
			if (x - mx == 0 && board[mx - 1][my - 1] == 'W')
			{
				return false;
			}
			if (x - mx == 1 && (my - y == 1 || my - y == -1) && board[mx - 1][my - 1] == 'O')
			{
				return false;
			}
			return true;
		}
		return false;
	}
	return true;
}
//Computer가 Pawn을 움직임
//Color값만 인자로 받음
int ComMovePawn(string ComColor)
{
	int result = 0;

	if (ComColor.compare("White") == 0)
	{	//White일 때는 Max start
		result = AlphaBetaMax(tree[0], -999, 999);
		tree[0] = tree[1]; //tree 이동 (다음 단계로 갱신)
		tree[0]->setLev(0);
	}
	else
	{	//Black일 때는 Min start
		result = AlphaBetaMin(tree[0], -999, 999);
		tree[0] = tree[1];
		tree[0]->setLev(0);
	}
	return result;
}
//User가 Pawn을 움직일 때
//game tree를 인자로 받음
int UseMovePawn(vector<Data*> tree)
{
	int x = -1; //움직일 pawn의 x좌표
	int y = -1; //움직일 pawn의 y좌표
	int mx = -1; //움직일 곳의 x좌표
	int my = -1; //움직일 곳의 y좌표
	int result = 0;
	vector<pair<int, int>> curloc;
	char ucolor = 0;
	vector<vector<char>> board = tree[0]->getBoard();
	if (tree[0]->getColor()[0] == 'B')
	{
		ucolor = 'W';
	}
	else
	{
		ucolor = 'B';
	}
	//user pawn이 있는 위치에 대한 x, y좌표 받기
	curloc = getlocation(tree[0]->getBoard(), ucolor);
	while (1)
	{
		cout << "Select Pawn(x,y)(ex:1 2)" << endl;
		cin >> x >> y;
		auto it = find(curloc.begin(), curloc.end(), make_pair(x - 1, y - 1));
		if (it == curloc.end())
		{
			cout << "Wrong Select" << endl;
			cout << "Select Again" << endl;
		}
		else
		{
			cout << "Select Move Point(x,y)(ex:1 2)" << endl;
			cin >> mx >> my;
			if (CheckPawn(board, x, y, mx, my, ucolor))
			{
				break;
			}
			else
			{
				cout << "Wrong Select" << endl;
				cout << "Select Again" << endl;
			}
		}
	}
	board[x - 1][y - 1] = 'O';
	board[mx - 1][my - 1] = ucolor;
	tree[0]->setBoard(board);
	//움직인 뒤의 board에 대한 value 계산
	result = CheckValue(tree[0]->getBoard());
	return result;

}
//FILE* pointer을 인자로 받고(txt파일) 2차원 vector를 반환
vector<vector<char>> LoadBoard(FILE* fin)
{
	vector<vector<char>> loadboard;
	vector<char> line;
	while (1)
	{
		char arr[10];
		char* str = fgets(arr, 10, fin);
		if (str == NULL)
		{
			break;
		}
		else
		{
			string astr = str;

			for (int i = 0; line.size() != 3; i++)
			{
				if (astr[i] == '1')
					line.push_back('W');
				else if (astr[i] == '2')
					line.push_back('B');
				else if (astr[i] == '0')
					line.push_back('O');
			}
		}
		loadboard.push_back(line);
		line.clear();
	}
	return loadboard;
}
int main(void)
{
	int exit = 1;
	//program run
	while (exit != 2)
	{	//current state
		Data* c_state = new Data;
		//tree create
		tree.push_back(c_state);
		tree.push_back(c_state); //n_state
		//Player Color
		string PlayerColor;
		//Computer Color
		string ComColor;
		//처음 할 색깔
		string first;
		//filename
		string filename;
		//file pointer
		FILE* fp;
		while (1)
		{
			cout << "Select Player Color(White or Black) :";
			cin >> PlayerColor;
			if (PlayerColor.compare("White") == 0)
			{	//Computer Color는 User Color의 반대
				ComColor = "Black";
				c_state->setColor(ComColor); //state의 color는 computer color로 설정(계산을 해야하므로)
				break;
			}
			else if (PlayerColor.compare("Black") == 0)
			{
				ComColor = "White";
				c_state->setColor(ComColor);
				break;
			}
			else
			{
				cout << "Wrong Select" << endl << "Select Again" << endl;
			}
		}
		while (1)
		{	//선공 선택
			cout << "Select first(White or Black) : ";
			cin >> first;
			if (first.compare("White") != 0 && first.compare("Black") != 0)
			{
				cout << "Wrong Select" << endl << "Select Again" << endl;
			}
			else
			{
				break;
			}

		}
		//상태판이 있는 파일 load
		cout << "Load file(default board=0)" << endl;
		cin >> filename;
		if (filename.compare("0") == 0)
		{
			cout << "Create Init Board" << endl;
			c_state->setBoard(InitBoard());
		}
		else
		{
			while (1)
			{
				fp = fopen(filename.c_str(), "r");
				if (fp == NULL)
				{
					cout << "Wrong file name" << endl;
				}
				else
				{
					cout << "Load file" << endl;
					c_state->setBoard(LoadBoard(fp));
					break;
				}
			}
		}
		ShowBoard(c_state);
		int result = 0;
		if (first.compare(ComColor) == 0)
		{	//Computer가 선공일 때
			while (result != -100 && result != 100)
			{	//Computer 먼저 이동
				result = ComMovePawn(ComColor);
				tree[0]->setColor(ComColor);
				cout << "COMPUTER TURN" << endl;
				ShowBoard(tree[0]);
				//게임이 끝났는지 확인
				result = CheckValue(tree[0]->getBoard());
				if (result == -100 || result == 100)
				{
					break;
				}
				//User 이동
				cout << "USER TURN" << endl;
				result = UseMovePawn(tree);
				ShowBoard(tree[0]);
				//게임이 끝났는지 확인
				if (result == -100 || result == 100)
				{
					break;
				}
			}
		}
		else
		{
			while (result != -100 && result != 100)
			{	//User 먼저 이동
				cout << "USER TURN" << endl;
				result = UseMovePawn(tree);
				ShowBoard(tree[0]);
				//게임이 끝났는지 확인
				if (result == -100 || result == 100)
				{
					break;
				}
				//Computer 이동
				result = ComMovePawn(ComColor);
				tree[0]->setColor(ComColor);
				cout << "COMPUTER TURN" << endl;
				ShowBoard(tree[0]);
				result = CheckValue(tree[0]->getBoard());
				//게임이 끝났는지 확인
				if (result == -100 || result == 100)
				{
					break;
				}
			}
		}
		//게임이 끝났다면
		//누가 이겼는지 확인
		cout << "FINISH!" << endl;
		if (result == 100)
		{
			if (ComColor.compare("Black") == 0)
			{
				cout << "User Win" << endl;
			}
			else
			{
				cout << "Computer Win" << endl;
			}
		}
		else
		{
			if (ComColor.compare("Black") == 0)
			{
				cout << "Computer Win" << endl;
			}
			else
			{
				cout << "User Win" << endl;
			}
		}
		while (1)
		{	//더 할 것인지 물어봄
			cout << "Play more (1), Exit (2) : " << endl;
			cin >> exit;
			if (exit < 1 && exit > 2)
			{
				cout << "wrong input" << endl;
			}
			else
			{
				break;
			}
		}
		delete c_state;
		tree.clear();
	}
	cout << "Program End" << endl;
	return 0;
}