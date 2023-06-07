#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#pragma warning(disable:4996)
using namespace std;
//���� ���� ���� Data
class Data
{
private:
	string color; //���� �� ������ �Ǵ� color
	int level;	  //���� tree�� level
	int e_value;  //evaluation ��
	vector<vector<char>> board; //hexapawn board

public:
	Data() //constructor �ʱ�ȭ
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
	//�� private ������ ���� get, set �Լ� �ۼ�
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
//alpha beta prunning�� ���� minimax �Լ�
int AlphaBetaMax(Data* pNode, int alpha, int beta); //Max �� ���ϱ�
int AlphaBetaMin(Data* pNode, int alpha, int beta); //Min �� ���ϱ�
//Game Tree ���� ���� ����
vector<Data*> tree;
//���� �⺻���� Board ����
//2���� vector�� ���·� return
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
//2���� vector ������ board�� �ް�
//���� Board ��Ȳ�� �´� value ��� �� return
int CheckValue(vector<vector<char>> c_board)
{
	int value = 0;
	bool gameover = false;
	auto wcheck = find(c_board[2].begin(), c_board[2].end(), 'W'); //White pawn�� ���� �����ߴ��� ã�� ����
	auto bcheck = find(c_board[0].begin(), c_board[0].end(), 'B'); //Black pawn�� ���� �����ߴ��� ã�� ����
	int wnum = 0; //white pawn ī��Ʈ
	int bnum = 0; //black pawn ī��Ʈ
	int wvalue = 0; //white ���� evaluation ���
	int bvalue = 0; //black ���� evaluation ���
	//�� count �� evaluation ���
	for (int i = 0; i < 3; i++)
	{
		wnum += count(c_board[i].begin(), c_board[i].end(), 'W');
		bnum += count(c_board[i].begin(), c_board[i].end(), 'B');
		wvalue += count(c_board[i].begin(), c_board[i].end(), 'W') * (i + 1) * (i + 1);
		bvalue += count(c_board[i].begin(), c_board[i].end(), 'B') * (3 - i) * (3 - i);

	}
	//white pawn�� ���� �������� ���(white �¸�)
	if (wcheck != c_board[2].end())
	{
		return 100;
	}
	//black pawn�� ���� �������� ���(black �¸�)
	if (bcheck != c_board[0].end())
	{
		return -100;
	}
	//white pawn�� �� �׾��� ���(white �й�)
	if (wnum == 0)
	{
		return -100;
	}
	//black pawn�� �� �׾��� ���(black �й�)
	if (bnum == 0)
	{
		return 100;
	}
	//�� �̻� ������ �� ���� ��� �Ǻ�
	if (wnum == 1 && bnum == 2)
	{
		//white pawn�� �� �����̴� ���(white �й�)
		if (c_board[1][0] == 'W' && c_board[1][1] == 'B' && c_board[2][1] == 'B')
			return -100;
		if (c_board[1][1] == 'B' && c_board[1][2] == 'W' && c_board[2][2] == 'B')
			return -100;
	}
	if (wnum == 1 && bnum == 3)
	{	//white pawn�� �� �����̴� ���(white �й�)
		if (c_board[1][0] == 'B' && c_board[1][1] == 'W' && c_board[1][2] == 'B' && c_board[2][1] == 'B')
			return -100;
	}
	if (wnum == 2 && bnum == 1)
	{	//black pawn�� �� �����̴� ���(black �й�)
		if (c_board[0][0] == 'W' && c_board[1][0] == 'B' && c_board[1][1] == 'W')
			return 100;
		if (c_board[0][2] == 'W' && c_board[1][1] == 'W' && c_board[1][2] == 'B')
			return 100;
	}
	if (wnum == 3 && bnum == 1)
	{	//balck pawn�� �� �����̴� ���(black �й�)
		if (c_board[0][1] == 'W' && c_board[1][0] == 'W' && c_board[1][1] == 'B' && c_board[1][2] == 'W')
			return 100;
	}
	//evaluation value�� ��ȯ
	return wvalue - bvalue;
}
//tree�� Data�� �ް� Data�� Board�� ���
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
//pawn�� color�� board ���¸� �ް� �ش� x,y��ǥ�� ������ ����
vector<pair<int, int>> getlocation(vector<vector<char>> board, char color)
{	//x,y��ǥ ���� ������ vector
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
//AlphaBeta prunning�� ���� Max���� ����ϴ� function
//alpha���� beta���� �ް� Data�� ���� evaluation value�� ����Ͽ� alpha �� beta ��ȯ
//Recursive�ϰ� ����
//Max�� White ��ġ���� ���
int AlphaBetaMax(Data* pNode, int alpha, int beta)
{
	int temp;
	int value = CheckValue(pNode->getBoard()); //evaluation ���
	//�¸� �� �й谡 ���� ���� ��
	if (value == 100 || value == -100)
	{
		return value;
	}
	//3 ply�� ���� ���� ��
	if (pNode->getLev() == 3)
	{
		return value;
	}
	vector<vector<char>> cur_b; //current board
	vector<vector<char>> nex_b; //next board
	vector<pair<int, int>> comloc; //computer�� play�ϴ� ���� pawn x,y��ǥ ���
	vector<pair<int, int>> useloc; //player�� play�ϴ� ���� pawn x,y��ǥ ���
	char ccolor = pNode->getColor()[0]; //computer color ����
	char ucolor; //user color�� �ݴ�
	if (ccolor == 'B')
	{
		ucolor = 'W';
	}
	else
	{
		ucolor = 'B';
	}
	//current board �ޱ�
	cur_b = pNode->getBoard();
	//�� x, y��ǥ ��ġ �ޱ�
	comloc = getlocation(cur_b, ccolor);
	useloc = getlocation(cur_b, ucolor);
	//computer location�� �ް� �� state�� ���� evaluation value ���
	for (int i = 0; i < comloc.size(); i++)
	{	//computer location�� ���� user location ��ġ ã��
		auto it = find(useloc.begin(), useloc.end(), make_pair(comloc[i].first + 1, comloc[i].second - 1));
		//���� �밢��
		if (comloc[i].second - 1 >= 0 && comloc[i].first + 1 < 3)
		{	//���� �밢���� computer�� �ݴ�Ǵ� ������ ���� ���
			if (it != useloc.end())
			{	//board update
				nex_b = cur_b;
				nex_b[comloc[i].first][comloc[i].second] = 'O';
				nex_b[comloc[i].first + 1][comloc[i].second - 1] = ccolor;
				//���� Ʈ�� ���� ����
				Data* pNew = new Data;
				pNew->setBoard(nex_b);
				pNew->setLev(pNode->getLev() + 1);
				pNew->setColor("Black");
				temp = AlphaBetaMin(pNew, alpha, beta);
				//value�� alpha���� ũ�� alpha���� value�� ����
				if (temp > alpha)
				{
					alpha = temp;
					//level 1���� ������ state �� ��ȯ
					if (pNew->getLev() == 1)
					{	//alpha beta prunning
						tree[1] = pNew;
					}
				}
			}
		}
		//������ �밢���� computer�� �ݴ�Ǵ� ������ ���� ���
		it = find(useloc.begin(), useloc.end(), make_pair(comloc[i].first + 1, comloc[i].second + 1));
		if (comloc[i].second + 1 < 3 && comloc[i].first + 1 < 3)
		{
			if (it != useloc.end())
			{	//board ����
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
		//������ �̵� ������ ��
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
		//�������� �̵�
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
		//���������� �̵�
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
		//alpha���� beta�� �� �ϰ� beta ��ȯ
		if (alpha >= beta)
		{
			return beta;
		}

	}
	//alpha ��ȯ
	return alpha;
}
//Alpha Beta prunning�� ���� Min �� ���ϱ�
//alpha�� beta���� �ް� ���� Data�� ���� value ���
//Black��ġ���� value ���
//Max MIn�� �Դٰ����ϸ� recursive call
int AlphaBetaMin(Data* pNode, int alpha, int beta)
{
	int temp;
	//���� board�� ���� evaluation value ���
	int value = CheckValue(pNode->getBoard());
	//�¸� �� �й谡 ���� ���� ��
	if (value == 100 || value == -100)
	{
		return value;
	}
	//3 ply�� �������� �� value ��ȯ
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
	//computer loaction�� ���� state value ���(Black)
	for (int i = 0; i < comloc.size(); i++)
	{	//���� �밢�� ���
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
		//������ �밢�� ���
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
		//������ �̵�
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
		//���� �̵�
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
		//������ �̵�
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
		//alpha�� beta���� ũ��
		if (alpha >= beta)
		{	//alpha return
			return alpha;
		}

	}
	return beta;
}
//���� ��ġ�� ������ ��ġ �޾Ƽ� ������ �� �ִ��� Ȯ��
bool CheckPawn(vector<vector<char>> board, int x, int y, int mx, int my, char ucolor)
{	//������ �Ѵ��� Ȯ��
	if (mx > 3 || my > 3 || mx < 1 || my < 1)
	{
		return false;
	}
	if (my - y > 1 || y - my > 1)
	{
		return false;
	}
	//�� ��ġ�� ���� ��
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
//Computer�� Pawn�� ������
//Color���� ���ڷ� ����
int ComMovePawn(string ComColor)
{
	int result = 0;

	if (ComColor.compare("White") == 0)
	{	//White�� ���� Max start
		result = AlphaBetaMax(tree[0], -999, 999);
		tree[0] = tree[1]; //tree �̵� (���� �ܰ�� ����)
		tree[0]->setLev(0);
	}
	else
	{	//Black�� ���� Min start
		result = AlphaBetaMin(tree[0], -999, 999);
		tree[0] = tree[1];
		tree[0]->setLev(0);
	}
	return result;
}
//User�� Pawn�� ������ ��
//game tree�� ���ڷ� ����
int UseMovePawn(vector<Data*> tree)
{
	int x = -1; //������ pawn�� x��ǥ
	int y = -1; //������ pawn�� y��ǥ
	int mx = -1; //������ ���� x��ǥ
	int my = -1; //������ ���� y��ǥ
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
	//user pawn�� �ִ� ��ġ�� ���� x, y��ǥ �ޱ�
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
	//������ ���� board�� ���� value ���
	result = CheckValue(tree[0]->getBoard());
	return result;

}
//FILE* pointer�� ���ڷ� �ް�(txt����) 2���� vector�� ��ȯ
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
		//ó�� �� ����
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
			{	//Computer Color�� User Color�� �ݴ�
				ComColor = "Black";
				c_state->setColor(ComColor); //state�� color�� computer color�� ����(����� �ؾ��ϹǷ�)
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
		{	//���� ����
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
		//�������� �ִ� ���� load
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
		{	//Computer�� ������ ��
			while (result != -100 && result != 100)
			{	//Computer ���� �̵�
				result = ComMovePawn(ComColor);
				tree[0]->setColor(ComColor);
				cout << "COMPUTER TURN" << endl;
				ShowBoard(tree[0]);
				//������ �������� Ȯ��
				result = CheckValue(tree[0]->getBoard());
				if (result == -100 || result == 100)
				{
					break;
				}
				//User �̵�
				cout << "USER TURN" << endl;
				result = UseMovePawn(tree);
				ShowBoard(tree[0]);
				//������ �������� Ȯ��
				if (result == -100 || result == 100)
				{
					break;
				}
			}
		}
		else
		{
			while (result != -100 && result != 100)
			{	//User ���� �̵�
				cout << "USER TURN" << endl;
				result = UseMovePawn(tree);
				ShowBoard(tree[0]);
				//������ �������� Ȯ��
				if (result == -100 || result == 100)
				{
					break;
				}
				//Computer �̵�
				result = ComMovePawn(ComColor);
				tree[0]->setColor(ComColor);
				cout << "COMPUTER TURN" << endl;
				ShowBoard(tree[0]);
				result = CheckValue(tree[0]->getBoard());
				//������ �������� Ȯ��
				if (result == -100 || result == 100)
				{
					break;
				}
			}
		}
		//������ �����ٸ�
		//���� �̰���� Ȯ��
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
		{	//�� �� ������ ���
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