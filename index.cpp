#include <fstream>
#include <iostream>
#include <cstring>
#include <sstream>
using namespace std;
class TourBus
{
public:
    int bid;
    char destination[200];
    char time[50];
    int max_seats;
    int booked;
    int fare;
	public:
	TourBus()
	{
	bid=0;
	max_seats=50;
	booked=0;
	fare=0;
	strcpy(time,"9:10am");
	strcpy(destination,"");
	}
	void input();
	void show();
	void display();
	int getid()
	{
	return bid;
	}
	void book()
	{
	booked++;
	}
	char* getDestination()
	{
	return destination;
	}
	char* getTime()
	{
	return time;
	}
	int getBooked(){
		return booked;
	}
	int getMax(){
		return max_seats;
	}
	int getFare()
	{
		return fare;
	}
	void loadFromText(istream &fin) {
	    fin >> bid;
	    fin.ignore();
	    fin.getline(destination, 200, '\t');
	    fin.getline(time, 50, '\t');
	    fin >> max_seats >> booked >> fare;
	}
};
class Ticket
{
	char name[50];
	TourBus bus;
	public:

	void generate(char cname[], TourBus tb)
	{
		strcpy(name,cname);
		bus=tb;
	}
	void display()
	{
	cout<<"Customer Name "<<name<<endl;
	cout<<"Details of Bus "<<endl;
	bus.show();
		}
	char* getName() {
	    return name;
	}
	TourBus getBus() {
	    return bus;
	}
};
void TourBus::input()
{
	cout<<"Enter bus id ";
	cin>>bid;
	cout<<"Enter bus destination ";
	cin.ignore();
	cin.getline(destination, 200);
	cout<<"Enter time of bus ";
	cin>>time;
	cout<<"Enter fare of the bus ";
	cin>>fare;
}
void TourBus::display()
{
	cout<<bid<<"\t"<<destination<<"\t"<<time<<"\t"<<max_seats<<"\t"<<booked<<"\t"<<fare<<"\n";
}
void TourBus::show()
{
	cout<<"Bus Id "<<bid<<endl;
	cout<<"Desination "<<destination<<endl;
	cout<<"Time "<<time<<endl;
	cout<<"No. of seats remaining "<<max_seats-booked<<endl;
}
int main()
{
	int ch;
	int id,chk=0;
	Ticket t;
	fstream F,G;
	TourBus b;
	do
	{
		system("clear");
		cout << "\033[1;36m===============================\033[0m" << endl;
		cout << "\033[1;33m     TOUR BUS RESERVATION      \033[0m" << endl;
		cout << "\033[1;36m===============================\033[0m\n" << endl;
		cout << " \033[1;37m1.\033[0m Add a New Tour Bus" << endl;
		cout << " \033[1;37m2.\033[0m Show Selected Bus" << endl;
		cout << " \033[1;37m3.\033[0m Display All Buses" << endl;
		cout << " \033[1;37m4.\033[0m Delete a Bus" << endl;
		cout << " \033[1;37m5.\033[0m Book a Ticket" << endl;
		cout << " \033[1;37m6.\033[0m Display Booked Tickets" << endl;
		cout << " \033[1;37m7.\033[0m Exit" << endl;
		cout << "\n\033[1;36m===============================\033[0m" << endl;
		cout << "\033[1;101mEnter your choice (1-7): \033[0m";
		cin>>ch;
		switch(ch)
		{
		case 1:
			cout << "\n\033[1;35m----- Add a New Tour Bus -----\033[0m\n";
			F.open("tour.csv",ios::app);
			b.input();
			F << b.getid() << '\t' << b.getDestination() << '\t' << b.getTime()
			  << '\t' << b.getMax() << '\t' << b.getBooked() << '\t' << b.getFare() << '\n';
			F.close();
			cout << "\033[1;32mBus added Successfully\033[0m" << endl;
			cin.ignore();
			cin.get();
			system("clear");

		break;
		case 2:
			cout << "\n\033[1;35m----- Show Selected Bus -----\033[0m\n";
			chk=0;
			cout<<"Enter the bus id to be displayed ";
			cin>>id;
			F.open("tour.csv", ios::in);
			if(F.fail())
				cout << "\033[1;31mCan't open file\033[0m" << endl;
			else
			{
				char line[512];
				while(F.getline(line, 512)) {
					istringstream iss(line);
					iss >> b.bid;
					iss.ignore();
					iss.getline(b.destination, 200, '\t');
					iss.getline(b.time, 50, '\t');
					iss >> b.max_seats >> b.booked >> b.fare;

					if (b.getid() == id) {
						b.show();
						chk = 1;
						break;
					}
				}
				if(chk==0)
					cout<<"Bus not Found"<<endl;
			}
			F.close();
			cin.ignore();
			cin.get();
			break;


		case 3:
			cout << "\n\033[1;35m----- Display All Buses -----\033[0m\n";
			F.open("tour.csv",ios::in);
			if(F.fail())
				cout << "\033[1;31mCan't open file\033[0m" << endl;
			else
			{
				while (F.peek() != EOF) {
				    b.loadFromText(F);
				    if (F) b.display();
				}
			}
			F.close();
			cout << "\n";
			cout<<"Press a key to continue ";
			cin.ignore();
			cin.get();
			system("clear");
		break;
		case 4:
			cout << "\n\033[1;35m----- Delete a Bus -----\033[0m\n";
			chk=0;
			cout<<"Enter the bus id to be deleted ";
			cin>>id;
			F.open("tour.csv", ios::in);
			G.open("temp.txt", ios::out);
			if(F.fail())
				cout << "\033[1;31mCan't open file\033[0m" << endl;
			else
			{
				char line[512];
				while(F.getline(line, 512))
				{
					istringstream iss(line);
					iss >> b.bid;
					iss.ignore();
					iss.getline(b.destination, 200, '\t');
					iss.getline(b.time, 50, '\t');
					iss >> b.max_seats >> b.booked >> b.fare;

					if(b.bid != id)
					{
						G << b.getid() << '\t' << b.getDestination() << '\t' << b.getTime()
						  << '\t' << b.getMax() << '\t' << b.getBooked() << '\t' << b.getFare() << '\n';
					}
					else
					{
						b.show();
						chk = 1;
					}
				}
				if(chk==0)
					cout<<"Bus not Found"<<endl;
				else
					cout << "\033[1;32mBus Deleted\033[0m" << endl;
			}
			F.close();
			G.close();
			remove("tour.csv");
			rename("temp.txt","tour.csv");
			cin.ignore();
			cin.get();
			break;
		case 5:
			cout << "\n\033[1;35m----- Book a Ticket -----\033[0m\n";
			char dest[70],cname[50];
			chk=0;
			cout<<"Enter the destination ";
			cin.ignore();
			cin.getline(dest, 70);
			F.open("tour.csv",ios::in);
			if(F.fail())
				cout << "\033[1;31mCan't open file\033[0m" << endl;
			else
			{
				while(F)
				{
					char line[512];
					if(!F.getline(line, 512)) break;
					istringstream iss(line);
					iss >> b.bid;
					iss.ignore();
					iss.getline(b.destination, 200, '\t');
					iss.getline(b.time, 50, '\t');
					iss >> b.max_seats >> b.booked >> b.fare;

					if(strcmp(b.getDestination(),dest)==0 && chk==0)
					{
						b.show();
						chk=1;
						cout<<"Enter the customer name ";
						cin.getline(cname, 50);
						b.book();
						t.generate(cname,b);
						G.open("tickets.csv", ios::app);
						G << cname << '\t' << b.getid() << '\t' << b.getDestination() << '\t'
						  << b.getTime() << '\t' << b.getMax() << '\t' << b.getBooked() << '\t'
						  << b.getFare() << '\n';
						G.close();
						cout<<"Ticket booked"<<endl;
					}
				}
				F.clear();
				F.seekg(0);
				fstream Temp("temp.csv", ios::out);
				while (true) {
				    char line[512];
				    if (!F.getline(line, 512)) break;
				    istringstream iss(line);
				    int bid;
				    char dest2[200], time2[50];
				    int max_seats, booked, fare;
				    iss >> bid;
				    iss.ignore();
				    iss.getline(dest2, 200, '\t');
				    iss.getline(time2, 50, '\t');
				    iss >> max_seats >> booked >> fare;

				    if (strcmp(dest2, b.getDestination()) == 0 && bid == b.getid()) {
				        booked = b.getBooked();
				    }
				    Temp << bid << '\t' << dest2 << '\t' << time2 << '\t'
				         << max_seats << '\t' << booked << '\t' << fare << '\n';
				}
				F.close();
				Temp.close();
				remove("tour.csv");
				rename("temp.csv", "tour.csv");
				if(chk==0)
					cout<<"No Bus Found"<<endl;
			}
			break;
		case 6:
			cout << "\n\033[1;35m----- Display Booked Tickets -----\033[0m\n";
			cout<<"\n\t\033[91mBooked Tickets \033[0m\n"<<endl;
			G.open("tickets.csv", ios::in);
			if (G.fail())
			    cout << "\033[1;31mcan't open file\033[0m" << endl;
			else {
			    char name[50];
			    TourBus temp;
			    while (G.peek() != EOF) {
			        G.getline(name, 50, '\t');
			        G >> temp.bid;
			        G.ignore();
			        G.getline(temp.destination, 200, '\t');
			        G.getline(temp.time, 50, '\t');
			        G >> temp.max_seats >> temp.booked >> temp.fare;
			        G.ignore();
			        t.generate(name, temp);
			        t.display();
					cout << "\n";
			    }
			}
			G.close();
			cout << "\n";
			cout<<"\033[44mPress a key to continue\033[0m ";
			cin.ignore();
			cin.get();
			break;
		case 7:
			break;
		}
	}while(ch!=7);
	return 0;
}