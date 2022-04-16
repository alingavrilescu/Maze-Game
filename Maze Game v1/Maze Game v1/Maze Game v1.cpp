#include <iostream>
#include <conio.h>
#include <fstream>
#include <string>
#include <cstdlib>

using namespace std;

#define KEY_UP    72
#define KEY_LEFT  75
#define KEY_RIGHT 77
#define KEY_DOWN  80
int GameOver = 0; //Var pentru conditie terminare joc.
int fate = 0; //Var pentru a decide outcomeul jocului.
int choice = 0;//Var pentru decizii in labirint
class Maze;

class Player
{
private:
	int strenght = 0;
	int gold = 0;
	int x = 0, y = 9; // Coordonatele de unde incepe player.
	int keys = 0;
	int lantern = 0;
	int shaman = 0;
	int enemy = 0;
public:
	friend class Maze;
	Player()
	{

	}
	void setStrenght(int strenght)
	{
		this->strenght = strenght;
	}
	void setGold(int gold)
	{
		this->gold = gold;
	}
	int getStrenght()
	{
		return this->strenght;
	}
	int getGold()
	{
		return this->gold;
	}
	int getShaman()
	{
		return this->shaman;
	}
	int getEnemy()
	{
		return this->enemy;
	}
	int getKeys()
	{
		return this->keys;
	}
	int getLamp()
	{
		return this->lantern;
	}
	void Move(Player* p, Maze* m);
	int getX()
	{
		return this->x;
	}
	int getY()
	{
		return this->y;
	}
};

class Maze
{
private:
	int maze[21][21];
public:
	friend class Player;
	Maze(int a[21][21])
	{
		for (int i = 0;i <= 20;i++)
			for (int j = 0;j <= 20;j++)
				this->maze[i][j] = a[i][j];
	}
	void displayMaze(Player* p)
	{
		cout << "The current situation in the maze is: \n";

		if (p->getLamp() == 1 && p->getX() > 1)
			for (int i = p->getX() - 1;i <= p->getX() + 1;i++)
				for (int j = p->getY() - 1;j <= p->getY() + 1;j++)
					if (this->maze[i][j] >= 100)
						this->maze[i][j] = this->maze[i][j] - 100;

		for (int i = 0;i <= 20;i++)
		{
			switch (i)
			{
			case 1:
				cout << "\t\t Your current strenght is: " << p->getStrenght() << " str";
				break;
			case 4:
				cout << "\t\t Your inventory:";
				break;
			case 5:
				if (p->getGold())
					cout << "\t\t " << p->getGold() << " gold coins";
				break;
			case 6:
				if (p->getKeys())
					cout << "\t\t " << p->getKeys() << " key(s)";
				break;
			case 7:
				if (p->getLamp())
					cout << "\t\t a lamp";
				break;
			}
			cout << endl;
			for (int j = 0;j <= 20;j++)
			{
				switch (this->maze[i][j])
				{
				case 0:
					cout << ".";
					break;
				case 1:
					cout << "#";
					break;
				case 2:
					cout << "P";
					break;
				case 3:
					cout << "+";
					break;
				case 4:
					cout << "C";
					break;
				case 5:
					cout << "E";
					break;
				case 6:
					cout << "B";
					break;
				case 7:
					cout << "=";
					break;
				case 8:
					cout << "$";
					break;
				case 9:
					cout << "S";
					break;
				case 10:
					cout << "L";
					break;
				case 11:
					cout << "X";
					break;
				case 12:
					cout << "T";
					break;
				}
				if (this->maze[i][j] >= 100)
					cout << "*";
			}
		}
	}
};

void Player::Move(Player* p, Maze* m)
{
	int i = this->x;
	int j = this->y;
	int c = 0;
	int var = 0;
	//Misacarea din sageti.
	switch (c = _getch())
	{
	case KEY_UP:
		i = x - 1;
		j = y;
		break;
	case KEY_DOWN:
		i = x + 1;
		j = y;
		break;
	case KEY_LEFT:
		i = x;
		j = y - 1;
		break;
	case KEY_RIGHT:
		i = x;
		j = y + 1;
		break;
	}

	//Conditie daca vrei sa iesi din labirint prin intrare.
	if (i < 0 || j < 0)
	{
		cout << "You can not turn back now.\n";
		cout << "Choose another direction. ";
		this->Move(p, m);
		var = 1;
	}

	switch (m->maze[i][j] % 100)
	{
		//Cazul in care intri intr-un zid.
	case 1:
		cout << "There is a wall in the direction you are trying to move.\n";
		cout << "Choose another direction. ";
		this->Move(p, m);
		break;

		//Cazul in care dai de o usa inchisa.
	case 3:
		if (p->keys > 0)
		{
			cout << "You found a locked door. You can unlock this door with one of your keys.\n";
			cout << "Type (1) if you want to unlock the door and continue your journey or (0) if you do not want to unlock the door: ";
			cin >> choice;
			if (choice)
			{
				cout << "You opened the door.\n";
				cout << "The key was destroyed in the process.\n";
				cout << "Press enter to continue.";
				m->maze[x][y] = 0;
				x = i;
				y = j;
				m->maze[x][y] = 2;
				p->keys--;
				this->Move(p, m);
			}
			else
			{
				cout << "You decided to keep the door closed. You can come back later.";
				this->Move(p, m);
			}
		}
		else
		{
			cout << "You found a locked door.\n";
			cout << "You do not have any keys at the moment. Come back later after you get one.\n";
			cout << "Press enter to continue.";
			this->Move(p, m);
		}
		break;

		//Cazul in care gasesti o cheie.
	case 4:
		p->keys++;
		m->maze[x][y] = 0;
		x = i;
		y = j;
		m->maze[x][y] = 2;
		cout << "You found a key. This is an universal key that will unlock any door.\n";
		cout << "The key was added to your invetory.\n";
		cout << "Press enter to continue.";
		this->Move(p, m);
		break;

		//Cazul in care intalnesti un Bandit.
	case 5:
		cout << "You encountered a bandit.\n";
		cout << "A fight begins.\n";
		if (p->strenght >= 10)
		{
			cout << "You won the fight. The bandit flee into the maze. He will not bother you again.\n";
			m->maze[x][y] = 0;
			x = i;
			y = j;
			m->maze[x][y] = 2;
			p->enemy++;
			cout << "Press enter to continue.";
			this->Move(p, m);
		}
		else
		{
			if (p->gold >= 5)
			{
				cout << "The bandit was stronger then you and you lost the fight.\n";
				cout << "He stole 5 gold coins from you before you were able to run away.\n";
				p->gold = p->gold - 5;
				cout << "Press enter to continue.";
				this->Move(p, m);
			}
			else
			{
				GameOver = 1;
				fate = 1;
				cout << "The bandit was stronger then you and you lost the fight.\n";
				cout << "Since you did not have anymore money in your inventory, the bandit killed you.\n";
				cout << "Press enter to continue.";
				this->Move(p, m);
			}
		}
		break;

		//Cazul in care intalnesti Seful Banditilor.
	case 6:
		cout << "You encountered a The Bandits' Boss.\n";
		cout << "A fight begins.\n";
		if (p->strenght >= 25)
		{
			cout << "You won the fight. The Bandits' Boss flee into the maze. He will not bother you again.\n";
			m->maze[x][y] = 0;
			x = i;
			y = j;
			m->maze[x][y] = 2;
			p->enemy++;
			cout << "Press enter to continue.";
			this->Move(p, m);
		}
		else
		{
			GameOver = 1;
			fate = 1;
			cout << "The Bandits' Boss was much stronger then you and you lost the fight.\n";
			cout << "He executed you right after the fight.\n";
			cout << "Press enter to continue.";
			this->Move(p, m);
		}
		break;

		//Cazul in care gasesti un Cufar mic.
	case 7:
		cout << "You found a small chest.\n";
		cout << "You open it and find 5 gold coins inside.\n";
		p->gold = p->gold + 5;
		m->maze[x][y] = 0;
		x = i;
		y = j;
		m->maze[x][y] = 2;
		cout << "Press enter to continue.";
		this->Move(p, m);
		break;

		//Cazul in care gasesti un Cufar mare.
	case 8:
		cout << "You found a big chest.\n";
		cout << "You open it and find 20 gold coins inside.\n";
		p->gold = p->gold + 20;
		m->maze[x][y] = 0;
		x = i;
		y = j;
		m->maze[x][y] = 2;
		cout << "Press enter to continue.";
		this->Move(p, m);
		break;

		//Cazul in care gasesti un Shaman.
	case 9:
		cout << "You found a Shaman.\n";
		cout << "This Shaman can increase your strenght (+5 str) if you are willing to pay 10 gold coins.\n";
		if (p->gold >= 10)
		{
			cout << "Type (1) if you accept the Shaman's offer or (0) if you do not. You can come back later.\n";
			cin >> choice;
			if (choice)
			{
				cout << "You accepted the Shaman's offer, your strenght increased.\n";
				cout << "The Shaman disappeared after you paid his due.\n";
				p->strenght = p->strenght + 5;
				p->gold = p->gold - 10;
				p->shaman++;
				m->maze[x][y] = 0;
				x = i;
				y = j;
				m->maze[x][y] = 2;
				cout << "Press enter to continue.";
				this->Move(p, m);
			}
			else
			{
				cout << "You did not accept the Shaman's offer, you may come later.\n";
				cout << "Press enter to continue.";
				this->Move(p, m);
			}
		}
		else
		{
			cout << "You do not have enough gold coins to accept the Shaman's offer right now. You may come later.\n";
			cout << "Press enter to continue.";
			this->Move(p, m);
		}
		break;

		//Cazul in care gasesti un felinar.
	case 10:
		cout << "You found a lamp.\n";
		cout << "You put the lamp into your inventory, it might be handy later.\n";
		p->lantern++;
		m->maze[x][y] = 0;
		x = i;
		y = j;
		m->maze[x][y] = 2;
		cout << "Press enter to continue.";
		this->Move(p, m);
		break;
		//Cazul in care ai gasit iesirea din labirint.
	case 11:
		GameOver = 1;
		cout << "You succeded to complete the maze.\n";
		cout << "Press enter to continue.";
		this->Move(p, m);
		break;
		//Cazul in care ai gasit un teleport
	case 12:
		cout << "You found a teleport. This teleport can teleport you to the beggining of the maze.\n";
		cout << "Do you wish to teleport now? You can use it later.\n";
		cout << "Type (1) for yes and (0) for no.\n";
		cin >> choice;
		if (choice)
		{
			m->maze[x][y] = 0;
			x = 0;
			y = 9;
			m->maze[x][y] = 2;
			cout << "Press enter to continue.";
			this->Move(p, m);
		}
		else
		{
			cout << "You decided not to use the teleport for now. You may come and use it later.\n";
			m->maze[x][y] = 0;
			x = 19;
			y = 2;
			m->maze[x][y] = 2;
			cout << "Press enter to continue.";
			this->Move(p, m);
		}
		break;


	case 0:
		if (var == 0)
		{
			m->maze[x][y] = 0;
			x = i;
			y = j;
			m->maze[x][y] = 2;
		}
		break;
	}
}

void MeniuOutput()
{
	cout << "Welcome to THE MAZE GAME\n\n";
	cout << "Meniu:\n";
	cout << "1.Start Game\n";
	cout << "2.Tutorial\n";
	cout << "3.Quit Game\n";
}

int main()
{
	do
	{
		GameOver = 0; 
		fate = 0;
		MeniuOutput();
		int meniu;
		Player* player = new Player(); //Crearea jucatorului
		Maze* m;
		fstream tutorial;
		cout << "\nSelect your choice: ";
		cin >> meniu;
		switch (meniu)
		{
		case 1:
			system("cls");
			int c[21][21];
			cout << "EASY(1) \t\t\t\t\t HARD(2)\n";
			cout << "Initial Power:20 Str \t\t\t\t Initial Power:5 Str\n";
			cout << "Initial Gold:20 g \t\t\t\t Initial Gold:10 g \n";
			cout << "A map of the entire maze. \t\t\t An incomplete map of the maze. \n\n";
			int dif;
			cout << "Select your difficulty(1/2):" << endl;
			cin >> dif;
			//Setarea jocului in fct de dificultate
			if (dif == 1)
			{
				player->setStrenght(20);
				player->setGold(20);
				int a[21][21] =
				{
					{1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
					{1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
					{1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1},
					{1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 4, 0, 0, 0, 0, 0, 0, 1, 0, 1},
					{1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1},
					{1, 0, 0, 0, 0, 4, 1, 0, 1, 0, 0, 0, 1, 9, 0, 0, 0, 0, 0, 0, 1},
					{1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 3, 1, 1, 1, 1, 1},
					{1, 9, 0, 0, 0, 0, 1, 0, 1, 4, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
					{1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1},
					{1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 7, 0, 0, 1, 0, 1},
					{1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1},
					{1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 8, 1, 0, 1, 0, 0, 0, 0, 0, 1},
					{1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 3, 1, 0, 1, 1, 1, 1, 1},
					{1, 0, 0, 7, 1, 0, 1,10, 0, 0, 1, 0, 1, 0, 1, 5, 1, 0, 0, 0, 1},
					{1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
					{1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 6, 1, 0, 0, 0, 1, 0, 1},
					{1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1},
					{1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1},
					{1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1},
					{1, 12, 0, 5, 0, 3, 0, 0, 1, 9, 1, 0, 1, 9, 0, 0, 1, 0, 0, 0, 1},
					{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,11, 1, 1, 1, 1, 1, 1, 1, 1, 1}
				};
				for (int i = 0;i < 21;i++)
					for (int j = 0;j < 21;j++)
						c[i][j] = a[i][j];
			}
			else
				if (dif == 2)
				{
					player->setStrenght(5);
					player->setGold(10);
					int a[21][21] =
					{
						{1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
						{1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
						{1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1},
						{1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 4, 0, 0, 0, 0, 0, 0, 1, 0, 1},
						{1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1},
						{1, 0, 0, 0, 0, 4, 1, 0, 1, 0, 0, 0, 1, 9, 0, 0, 0, 0, 0, 0, 1},
						{1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 3, 1, 1, 1, 1, 1},
						{1, 9, 0, 0, 0, 0, 1, 0, 1, 4, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
						{1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 101, 101, 101, 101, 100, 101, 100, 101},
						{1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 100, 101, 107, 100, 100, 101, 100, 101},
						{1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 100, 101, 101, 101, 101, 101, 100, 101},
						{1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 8, 1, 100, 101, 100, 100, 100, 100, 100, 101},
						{1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 103, 101, 100, 101, 101, 101, 101, 101},
						{1, 0, 0, 7, 1, 0, 1,10, 0, 0, 1, 0, 1, 100, 101, 105, 101, 100, 100, 100, 101},
						{1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 100, 101, 100, 101, 100, 101, 100, 101},
						{1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 106, 101, 100, 100, 100, 101, 100, 101},
						{1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 100, 101, 101, 101, 101, 101, 100, 101},
						{1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 100, 101, 100, 100, 100, 101, 100, 101},
						{1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 101, 101, 100, 101, 100, 101, 100, 101},
						{1, 12, 0, 5, 0, 3, 0, 0, 1, 9, 1, 0, 1, 109, 100, 100, 101, 100, 100, 100, 101},
						{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,11, 1, 101, 101, 101, 101, 101, 101, 101, 101}
					};
					for (int i = 0;i < 21;i++)
						for (int j = 0;j < 21;j++)
							c[i][j] = a[i][j];
				}

			m = new Maze(c); // Crearea labirintului
			do
			{
				m->displayMaze(player);
				cout << "\n\n";
				player->Move(player, m);
				system("cls");
			} while (GameOver == 0);
			cout << "GAME OVER\n\n";
			if (fate)
			{
				cout << "You died\n\n";
				cout << "Score: 0\n";
			}
			else
			{
				cout << "You escaped the maze\n\n";
				cout << "What you achieved in your adventure: \n";
				cout << player->getShaman() << " Shaman(s) increased your power. Final strenght: " << player->getStrenght() << " str.\n";
				cout << "You won " << player->getEnemy() << " battle(s) against enemeies.\n";
				cout << "You escaped the maze with " << player->getGold() << " gold coins.\n\n";
				if (dif == 1)
					cout << "Final Score: " << player->getStrenght() + player->getGold() << endl;
				else
					if (dif == 2)
						cout << "Final Score: " << (player->getStrenght() + player->getGold()) * 4<<endl;
			}
			system("pause");
			system("cls");
			break;

		case 2:
			system("cls");
			tutorial.open("Tutorial.txt", ios::in);
			if (tutorial.is_open()) {
				string tp;
				while (getline(tutorial, tp)) {
					cout << tp << "\n";
				}
				tutorial.close();
				cout << endl;
				system("pause");
				system("cls");
			}
			break;

		case 3:
			system("cls");
			cout << "Thank you for playing my game.\n";
			exit(1);
		}
	} while (1);
}
