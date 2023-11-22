#include<iostream>
#include<vector>
#include<conio.h>
#include<ctype.h>
#include<stdlib.h>
#include<Windows.h>
#include<time.h>
constexpr auto DELAY = 150;
constexpr auto MAX_LINHAS = 10;
constexpr auto MAX_COLUNAS = 20;

char mapa[MAX_LINHAS][MAX_COLUNAS];
bool acabou = false;

class snake final
{	
public:
	int x;
	int y;
	char ultima_jogada;
	std::vector<snake> body;
	snake(void)
	{
		this->x = 1;
		this->y = 1;
		this->ultima_jogada = 'D';
	}
	snake(int x, int y) : x(x), y(y),ultima_jogada('D')
	{
	}
};

void build_map(void)
{
	for (int i = 0; i < MAX_LINHAS; i++)
	{
		for (int j = 0; j < MAX_COLUNAS; j++)
		{
			if (i == 0 || i == MAX_LINHAS - 1 || j == 0 || j == MAX_COLUNAS - 1) mapa[i][j] = '#';
			else mapa[i][j] = '.';
		}
	}
}

void rebuild_map(void)
{
	for (int i = 0; i < MAX_LINHAS; i++)
	{
		for (int j = 0; j < MAX_COLUNAS; j++)
		{
			if (i == 0 || i == MAX_LINHAS - 1 || j == 0 || j == MAX_COLUNAS - 1) mapa[i][j] = '#';
		}
	}
}

void show_map(void)
{
	system("cls");
	for (int i = 0; i < MAX_LINHAS; i++)
	{
		for (int j = 0; j < MAX_COLUNAS; j++)
		{
			std::cout << mapa[i][j];
		}
		std::cout << std::endl;
	}
}

void gera_fruta(void)
{
	bool pode_criar = false;
	int x, y;
	while (!pode_criar)
	{
		x = rand() % MAX_LINHAS;
		y = rand() % MAX_COLUNAS;
		if (mapa[x][y] == '.')
		{
			mapa[x][y] = '*';
			pode_criar = true;
		}
	}
}

void venceu(void)
{
	bool achou = false;
	for (int i = 0; i < MAX_LINHAS; i++)
	{
		for (int j = 0; j < MAX_COLUNAS; j++)
		{
			if (mapa[i][j] == '.') achou  = true;
		}
	}
	if (!achou)
	{
		std::cout << std::endl;
		std::cout << "Voce venceu" << std::endl;
		exit(1);
	}
}

void aux_map(void) 
{
	for (int i = 0; i < MAX_LINHAS; i++)
	{
		for (int j = 0; j < MAX_COLUNAS; j++)
		{
			if (mapa[i][j] == '*')
			{
				return;
			}
		}
	}
	gera_fruta();
}

void cria_corpo(snake& s, int ultimo_x, int ultimo_y)
{
	if (s.body.empty())
	{
		snake new_body(ultimo_x, ultimo_y);
		s.body.push_back(new_body);
		mapa[ultimo_x][ultimo_y] = 'O';
	}
	else 
	{
		int tamanho_corpo = s.body.size();
		int x_temp = s.body[static_cast<std::vector<snake, std::allocator<snake>>::size_type>(tamanho_corpo) - 1].x;
		int y_temp = s.body[static_cast<std::vector<snake, std::allocator<snake>>::size_type>(tamanho_corpo) - 1].y;

		switch (s.body[tamanho_corpo - 1].ultima_jogada)
		{
			case 'A':
				x_temp++;
				break;
			case 'W':
				x_temp--;
				break;
			case 'S':
				y_temp++;
				break;
			case 'D':
				y_temp--;
				break;
		}

		snake new_body(x_temp, y_temp);
		s.body.push_back(new_body);
		mapa[x_temp][y_temp] = 'O';
	}
}

void move_corpo(snake& s)
{
	if (s.body.empty()) return;

	int prevX = s.x;
	int prevY = s.y;
	char prevDirection = s.ultima_jogada;

	for (int i = 0; i < s.body.size(); i++)
	{
		int tempX = s.body[i].x;
		int tempY = s.body[i].y;
		char tempDirection = s.body[i].ultima_jogada;

		s.body[i].x = prevX;
		s.body[i].y = prevY;
		s.body[i].ultima_jogada = prevDirection;

		mapa[prevX][prevY] = 'O';

		prevX = tempX;
		prevY = tempY;
		prevDirection = tempDirection;
	}

	mapa[prevX][prevY] = '.'; 
	mapa[s.x][s.y] = '@';
}

void move_snake(snake& s,char direction)
{
	direction = toupper(direction);
	int oldX = s.x;
	int oldY = s.y;

	switch (direction)
	{
		case 'A':
			if (mapa[s.x][s.y - 1] == '#' || mapa[s.x][s.y - 1] == 'O') exit(1);
			if (mapa[s.x][s.y - 1] == '*')
			{
				gera_fruta();
				cria_corpo(s, s.x, s.y);
			}
			s.y -= 1;
			break;
		case 'W':
			if (mapa[s.x - 1][s.y] == '#' || mapa[s.x - 1][s.y] == 'O') exit(1);
			if (mapa[s.x - 1][s.y] == '*')
			{
				gera_fruta();
				cria_corpo(s, s.x, s.y);
			}
			s.x -= 1;
			break;
		case 'S':
			if (mapa[s.x + 1][s.y] == '#' || mapa[s.x + 1][s.y] == 'O') exit(1);
			if (mapa[s.x + 1][s.y] == '*')
			{
				gera_fruta();
				cria_corpo(s, s.x, s.y);
			}
			s.x += 1;
			break;
		case 'D':
			if (mapa[s.x][s.y + 1] == '#' || mapa[s.x][s.y + 1] == 'O') exit(1);
			if (mapa[s.x][s.y + 1] == '*')
			{
				gera_fruta();
				cria_corpo(s, s.x, s.y);
			}
			s.y += 1;
			break;
	}

	mapa[oldX][oldY] = '.';
	mapa[s.x][s.y] = '@';
	s.ultima_jogada = direction;

	if (s.body.empty())
	{
		snake new_body(s.x, s.y);
		s.body.push_back(new_body);
		mapa[s.x][s.y] = 'O';
	}

}

int main(void)
{
	srand(time(NULL));
	snake s;
	build_map();
	gera_fruta();
	char jogada = 'D';

	while (!acabou)
	{
		show_map();
		Sleep(DELAY);
		while (!_kbhit())
		{
			move_snake(s, jogada);
			move_corpo(s);
			venceu();
			aux_map();
			rebuild_map();
			show_map();
			Sleep(DELAY);
		}
		jogada = _getch();
		move_snake(s,jogada);
		move_corpo(s);
		venceu();
		aux_map();
		rebuild_map();
	}

	return 0;
}