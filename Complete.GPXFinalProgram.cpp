/******************************************************************************
Evan Sutton
CSI Senior Project
This Program works with .GPX navigational waypoints. It allows users to 
store waypoints from a file, create new waypoints, and create a new .GPX 
file using the stored waypoints.
*******************************************************************************/

#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <vector>

using namespace std;

struct wayPoint
{
    string name, latitude, longitude,
    symbol, date, exactTime;
};

string getTime();
void writeGPX(vector<wayPoint> wpt);
void readGPX(vector<wayPoint>& wpt);
void manualWpt(vector<wayPoint>& wpt);
void displayWpt(vector<wayPoint> wpt);
string validateAndFormatCoordinate(const string& input, bool isLatitude);
bool isValidName(const string& name);
string getCurrentDate();
string getCurrentTime();

int main()
{
    string name;
    int option;
    
    vector<wayPoint> wpt;
    
    cout <<  "1: Store waypoints from a .GPX file\n"
         << "2: Write a new .GPX file with stored waypoints\n"
         << "3: Create a new waypoint\n"
         << "4: Display all saved waypoint data\n"
         << "5: Exit\n";
    
    while(option != 4)
    {
        
        cout << "Please select an option: ";
        cin >> option;
        cout << "\n";
        
        if(option == 1)
        {
            readGPX(wpt);
        }
        else if(option == 2)
        {
            writeGPX(wpt);
        }
        else if(option == 3)
        {
            manualWpt(wpt);
        }
        else if(option == 4)
        {
            displayWpt(wpt);
        }
        else if(option == 5)
        {
            return 0;
        }
        else
            cout << "Invalid Entry\n\n";
    }
    
    return 0;
}

/*///////////////////////////////////////////////
Function create a wapyoint by inserting data
*////////////////////////////////////////////////
void manualWpt(vector<wayPoint>& wpt)
{
    wayPoint temp;
    string input;
    
    cout << "Enter Waypoint name: ";
    cin >> temp.name; //get name
    //Validate name input
    if (!isValidName(temp.name)) 
    {
        cout << "Error: Name must be at most 11 characters.\n\n";
        return;
    }
    
    cout << "Latitude: ";
    cin >> input;
    //validate latitude input
    try 
    {
        temp.latitude = validateAndFormatCoordinate(input, true);
    } 
    catch (const exception& e) 
    {
        cout << "Error: " << e.what() << "\n";
        return;
    }

    cout << "Longitude: ";
    cin >> input;
    //validate longitude input
    try 
    {
        temp.longitude = validateAndFormatCoordinate(input, false);
    } 
    catch (const exception& e) 
    {
        cout << "Error: " << e.what() << "\n";
        return;
    }
    
    temp.date = getCurrentDate();   //set date to current date
    temp.exactTime = getCurrentTime();   //set time to current time
    temp.symbol = "Yellow Square"; //All manual waypoints will default to a yellow square symbol
    
    wpt.push_back(temp);
    
    cout << "Waypoint successfully created\n\n";
}

/*///////////////////////////////////////////////
Function to display the waypoints stored in 
the vector
*////////////////////////////////////////////////
void displayWpt(vector<wayPoint> wpt)
{
    for(int i = 0; i < wpt.size(); i++)
    {
        cout << "Name: " << wpt[i].name
             << "\nLatitude: " << wpt[i].latitude
             << "\nLongitude: " << wpt[i].longitude
             << "\nDate: " << wpt[i].date
             << "\nTime: " << wpt[i].exactTime
             << "\nSymbol: " << wpt[i].symbol
             << "\n\n";
    }
}

/*///////////////////////////////////////////////
Function to open the wpt file and store the data
into a vector
*////////////////////////////////////////////////
void readGPX(vector<wayPoint>& wpt)
{
    int i = 0;
    
    wayPoint temp;
    string name;
    
    cout << "Enter file name: ";
    cin >> name;
    
    ifstream file(name);
    string target = "</metadata>";
    string target2 = "</wpt>";

    // Check if the file is successfully opened
    if (!file.is_open()) 
    {
        cout << "Error opening the file!\n\n";
        return;
    }

    // String variable to store the read data
    string s;

    // Read each line of the file and print it to the
    // standard output stream till the whole file is
  	// completely read
    while (getline(file, s))
    {
        if(s.find(target) != std::string::npos || s.find(target2) != std::string::npos)
        {
            getline(file, s, '"');
            getline(file, s, '"');
            temp.latitude = s;
            getline(file, s, '"');
            getline(file, s, '"');
            temp.longitude = s;
            getline(file, s);
            getline(file, s, '>');
            getline(file, s, 'T');
            temp.date = s;
            getline(file, s, 'Z');
            temp.exactTime = s;
            getline(file, s);
            getline(file, s, '>');
            getline(file, s, '<');
            temp.name = s;
            getline(file, s);
            getline(file, s, '>');
            getline(file, s, '<');
            temp.symbol = s;
            i++;
            wpt.push_back(temp);
        }
    }
    wpt.pop_back();
    //done reading, so close file    
    file.close();
    
    cout << "Waypoints successfully stored\n\n";
}


/*////////////////////////////////////////////////////
function that will find the current time and return it 
in a certain format
*/////////////////////////////////////////////////////
string getTime()
{
    string currentTime;
    
    auto t = time(nullptr);
    auto tm = *localtime(&t);

    ostringstream ct;
    ct << put_time(&tm, "<time>%Y-%m-%dT%H:%M:%SZ</time>");
    currentTime = ct.str();

    return currentTime;
}

/*////////////////////////////////////////////////////
Function that creates a new file and writes our vector
data to it
*/////////////////////////////////////////////////////
void writeGPX(vector<wayPoint> wpt)
{
    const wayPoint& a = wpt[0];
    ofstream file;

    file.open("Waypoints.GPX");
    
    if (!file.is_open()) 
    {
        cout << "Error opening the file!\n\n";
        return;
    }
    //Starting block of general file data
    file << "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n"
         << "<gpx creator=\"ECHOMAP Ultra 126sv\" version=\"1.1\" xmlns=\"http://www.topografix.com/GPX/1/1\" xmlns:gpxx=\"http://www.garmin.com/xmlschemas/GpxExtensions/v3\" xmlns:wptx1=\"http://www.garmin.com/xmlschemas/WaypointExtension/v1\" xmlns:gpxtpx=\"http://www.garmin.com/xmlschemas/TrackPointExtension/v1\" xmlns:uuidx=\"http://www.garmin.com/xmlschemas/IdentifierExtension/v1\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:schemaLocation=\"http://www.topografix.com/GPX/1/1 http://www.topografix.com/GPX/1/1/gpx.xsd http://www.garmin.com/xmlschemas/GpxExtensions/v3 http://www8.garmin.com/xmlschemas/GpxExtensionsv3.xsd http://www.garmin.com/xmlschemas/WaypointExtension/v1 http://www8.garmin.com/xmlschemas/WaypointExtensionv1.xsd http://www.garmin.com/xmlschemas/TrackPointExtension/v1 http://www.garmin.com/xmlschemas/TrackPointExtensionv1.xsd http://www.garmin.com/xmlschemas/IdentifierExtension/v1 http://www.garmin.com/xmlschemas/IdentifierExtension.xsd\">\n"
         << "\t<metadata>\n"
         << "\t\t<link href=\"http://www.garmin.com\">\n"
         << "\t\t\t<text>Garmin International</text>\n"
         << "\t\t</link>\n"
         << "\t\t" << getTime() << endl;    //get current time of file construction
         
    //First instance of waypoint entry
    file << "\t</metadata>\n"
         << "\t<wpt lat=\"" << a.latitude << "\" lon=\"" << a.longitude << "\">\n"
         << "\t\t<time>" << a.date << "T" << a.exactTime << "Z</time>\n"
         << "\t\t<name>" << a.name << "</name>\n"
         << "\t\t<sym>" << a.symbol << "</sym>\n";
         
    //loop to go through vector data and write it to file     
    for(int i = 1; i < wpt.size(); i++)
    {
        const wayPoint& b = wpt[i];
        file << "\t</wpt>\n"
             << "\t<wpt lat=\"" << b.latitude << "\" lon=\"" << b.longitude << "\">\n"
             << "\t\t<time>" << b.date << "T" << b.exactTime << "Z</time>\n"
             << "\t\t<name>" << b.name << "</name>\n"
             << "\t\t<sym>" << b.symbol << "</sym>\n";
    }
    //closing lines to mark the end of the file
    file << "\t</wpt>\n"
         << "</gpx>";
  
    file.close(); //close the new file
    
    cout << "File created successfully\n"
         << "Exit the program to view the new file.\n";
}

/*/////////////////////////////////////////////////////////
Function to validate Latitude and longitude inputs
*//////////////////////////////////////////////////////////
string validateAndFormatCoordinate(const string& input, bool isLatitude) {
    try 
    {
        double value = stod(input);
        
        if (isLatitude && (value < -90.0 || value > 90.0)) 
        {
            throw out_of_range("Latitude must be between -90 and 90.");
        }
        
        if (!isLatitude && (value < -180.0 || value > 180.0)) 
        {
            throw out_of_range("Longitude must be between -180 and 180.");
        }

        ostringstream out;
        out << fixed << setprecision(10) << value;
        return out.str();
    } 
    catch (...) 
    {
        throw invalid_argument("Invalid coordinate input.");
    }
}

/*/////////////////////////////////////////////////////////
Function to validate waypoint name
*//////////////////////////////////////////////////////////
bool isValidName(const string& name) 
{
    return name.length() <= 11;
}

/*/////////////////////////////////////////////////////////
Function to get current date
*//////////////////////////////////////////////////////////
string getCurrentDate() 
{
    time_t t = time(nullptr);
    tm* now = localtime(&t);

    ostringstream dateStream;
    dateStream << put_time(now, "%Y-%m-%d");

    return dateStream.str();
}

/*/////////////////////////////////////////////////////////
Function to get current time
*//////////////////////////////////////////////////////////
string getCurrentTime() 
{
    time_t t = time(nullptr);
    tm* now = localtime(&t);

    ostringstream timeStream;
    timeStream << put_time(now, "%H:%M:%S");

    return timeStream.str();
}