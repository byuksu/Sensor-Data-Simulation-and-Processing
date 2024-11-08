#include <iostream>
#include <random>
#include <queue>
#include <fstream> //dosya işlemleri
#include <unistd.h> //datayı periyodik veya rastgele bir zamanda almak için (usleep)

using namespace std;

enum data_generation_timing {
    periodically,
    asynchronously
};
enum filterType {
    SimpleAverageFilter,
    MovingAverageFilter
};

//parameters
static int windowSize = 3;
static int TimePeriod = 1000;
static int Npoints = 10;
static int dataRange =100;
static data_generation_timing dtg = periodically;
static filterType Ft = MovingAverageFilter;

void setParameters();
void printParameters();

string dataGenerationTimingToString(data_generation_timing dtg) {
    return (dtg == periodically) ? "periodically" : "asynchronously";
}

string filterTypeToString(filterType Ft) {
    return (Ft == SimpleAverageFilter) ? "SimpleAverageFilter" : "MovingAverageFilter";
}

template <class T>
class Sensor {
private:
    T data;
    data_generation_timing DGT;
    queue<T> myQueue; //Aynı zamanda rawData
public:
    Sensor(data_generation_timing mdgt = periodically) : DGT(mdgt) {}
    T generateData() {
        random_device rd;
        if (DGT == periodically)
            usleep((useconds_t)1000 * TimePeriod); // default : 1 saniyede bir data
        else if (DGT == asynchronously) {
            usleep((rd() % 1001) * TimePeriod); //default: 0 ile 1 saniye arasında değişen bir zamanda bir data
        }
        data = static_cast<T>(rd() % (20*dataRange+1)); //default:(-100,100)
        data -= (10*dataRange); 
        data /= 10; // sensorden gelen datanın int, float veya double olarak kullanmak için 
        cout << "Data received from the sensor: " << data << endl;
        return data;
    }
    void collectData(int numPoints) {
        if (numPoints <= 0)
            return;
        while (numPoints--) {
            myQueue.push(generateData());
            if(!numPoints) break;
        }
    }
    queue<T> getRawData() {
        return myQueue;
    }
};

template <class T>
class DataProcessor {
private:
    queue<T> filteredData;
    queue<T> rawData;
public:
    DataProcessor(int NumPoints = 0, data_generation_timing dtg = periodically) {
        Sensor<T> msensor(dtg);
        msensor.collectData(NumPoints);
        rawData = msensor.getRawData();
    };

    void filterData(filterType FType) {
        T temp = 0;
        int sizeofRawData = rawData.size();

        if (FType == SimpleAverageFilter) {
            if (rawData.empty()) {
                cerr << "No data in the container!" << endl;
                exit(1);
            }

            // Basit Ortalama Filtreleme
            while (!rawData.empty()) {
                temp += rawData.front();  // Veriyi topla
                rawData.pop();
            }
            temp /= sizeofRawData; // Ortalamayı al

            // Filtrelenmiş veriyi filteredData kuyruğuna ekle
            while (sizeofRawData) {
                filteredData.push(temp);
                if(!sizeofRawData) break;
            }
        }
        else if (FType == MovingAverageFilter) {
            int count = 0;
            T temp2;
            queue<T> window;
            while (windowSize - count) {
                if(rawData.empty()) break;
                window.push(rawData.front());
                rawData.pop();
                count++;
            }
            int move=rawData.size()+1;
            for (int j = 0; j < move; j++) {
                temp = 0;
                // Ortalamayı al
                for (int x = 0; x < windowSize; x++) {
                    if(window.empty())
                        break;
                    temp += window.front();
                    temp2 = window.front();
                    window.pop();
                    if (x)  // ilk değeri sil
                        window.push(temp2);
                }
                filteredData.push(temp / windowSize); // ortalamayı ekle
                if(rawData.empty()) break;
                window.push(rawData.front());
                rawData.pop();
            }
        }
    }
    T getMax(){
        T maxVal = -100;
        T temp;
        int size=filteredData.size();
        if(!size){
            cerr<<"Filtered Data is empty"<<endl;
            exit(1);
        }
        while(size--){
            temp=filteredData.front();
            filteredData.pop();
            filteredData.push(temp);
            if (temp > maxVal) maxVal = temp;
        }
        return maxVal;
    }

    T getMin(){
        T minVal = 100;
        T temp;
        int size=filteredData.size();
        if(!size){
            cerr<<"Filtered Data is empty"<<endl;
            exit(1);
        }
        while(size--){
            temp=filteredData.front();
            filteredData.pop();
            filteredData.push(temp);
            if (temp < minVal) minVal = temp;
        }
        return minVal;
    }

    T getAverage(){
        T temp=0, temp2=0;
        int size=filteredData.size();
        if(!size){
            cerr<<"Filtered Data is empty"<<endl;
            exit(1);
        }
        while(size--){
            temp2=filteredData.front();
            temp+=temp2;
            filteredData.pop();
            filteredData.push(temp2);
        }
        return (temp/filteredData.size());
    }
    
    void saveDatas(string filename){
        ofstream outfile(filename);
        if (!outfile) {
            cerr << "Could not open file!" << endl;
            return;
        }
        string text = "OUTPUTS\n" + string("*******************************************\n");
        text+="Number of Data Points  :"+to_string(Npoints)+"\n";
        text+="Minimum Value          :"+to_string(this->getMin())+"\n";
        text+="Maximum Value          :"+to_string(this->getMax())+"\n";
        text+="Average Value          :"+to_string(this->getAverage())+"\n";
        text+= "*******************************************";
        outfile << text; // Mevcut metni dosyaya yaz
        outfile.close(); // Dosyayı kapat
        //Text saved to output.txt
    }

    void printFilteredData() {
        if(!filteredData.size()){
            cerr<<"Filtered Data is empty"<<endl;
            exit(1);
        }
        cout << "=========================================" << endl;
        cout<<"Number of Data Points  :"<<Npoints<<endl;
        cout<<"Minimum Value          :"<<this->getMin()<<endl;
        cout<<"Maximum Value          :"<<this->getMax()<<endl;
        cout<<"Average Value          :"<<this->getAverage()<<endl;
        cout << "=========================================" << endl;
    }
};

int main() {

    cout<<"Default parameters: "<<endl;
    printParameters();
    char userChoice;
    cout << "Do you want to change parameters? (y/n): ";
    cin >> userChoice;

    if (userChoice == 'y' || userChoice == 'Y'){
        setParameters(); 
        cout<<"Updated parameters: "<<endl;
        printParameters();
    }
        
    DataProcessor<double> dataProcessor(Npoints, dtg);
    dataProcessor.filterData(Ft); // filtre uygula
    dataProcessor.printFilteredData(); // Filtrelenmiş verileri yazdır
    dataProcessor.saveDatas("output.txt"); // dataları kaydet

    return 0;
}

void setParameters()
{       

    // Update TimePeriod
    while (true) {
        cout << "Enter TimePeriod (positive integer in ms) less than 2000 (2s): ";
        cin >> TimePeriod;
        if (cin.fail() || TimePeriod <= 0 || TimePeriod >= 2000) {
            cin.clear(); // clear input buffer
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // ignore last input
            cout << "Invalid input." << endl;
        }
        else {
            break; // valid input, exit loop
        }
    }

    // Update Npoints
    while (true) {
        cout << "Enter Numpoints (positive integer) less than 200: ";
        cin >> Npoints;
        if (cin.fail() || Npoints <= 0 || Npoints >= 200) {
            cin.clear(); // clear input buffer
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // ignore last input
            cout << "Invalid input." << endl;
        }
        else {
            break; // valid input, exit loop
        }
    }

    // Update windowSize
     while (true) {
        cout << "Enter windowSize (positive integer), winndowsize should be less than Numpoints ("<<Npoints<<") : ";
        cin >> windowSize;
        if (cin.fail() || windowSize <= 0 || windowSize >= Npoints) {
            cin.clear(); // clear input buffer
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // ignore last input
            cout << "Invalid input." << endl;
        }
        else {
            break; // valid input, exit loop
        }
    }

    // Update dataRange
     while (true) {
        cout << "Enter dataRange (positive integer) ----> (-dataRange,dataRange) : ";
        cin >> dataRange;
        if (cin.fail() || dataRange <= 0) {
            cin.clear(); // clear input buffer
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // ignore last input
            cout << "Invalid input." << endl;
        }
        else {
            break; // valid input, exit loop
        }
    }

    // Update data_generation_timing
    while (true) {
        cout << "Enter data_generation_timing (0 for periodically, 1 for asynchronously): ";
        int dtgInput;
        cin >> dtgInput;
        if (cin.fail()) {
            cin.clear(); // hata bayrağını temizle
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // buffer'ı temizle
            cout << "Invalid input." << endl;
            continue; // döngünün başına dön
        }
        if (dtgInput == 0) {
            dtg = periodically;
            break;
        }
        else if (dtgInput == 1) {
            dtg = asynchronously;
            break;
        }
        else {
            cout << "Invalid input." << endl;
        }
    }

    // Update filterType
    while (true) {
        cout << "Enter filterType (0 for SimpleAverageFilter, 1 for MovingAverageFilter): ";
        int FtInput;
        cin >> FtInput;
        if (cin.fail()) {
            cin.clear(); // hata bayrağını temizle
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // buffer'ı temizle
            cout << "Invalid input." << endl;
            continue; // döngünün başına dön
        }
        if (FtInput == 0) {
            Ft = SimpleAverageFilter;
            break;
        }
        else if (FtInput == 1) {
            Ft = MovingAverageFilter;
            break;
        }
        else {
            cout << "Invalid input." << endl;
        }
    }
}

void printParameters() {
    cout << "--------------------------------------------------" << endl;
    cout << "windowSize = " << windowSize << endl;
    cout << "TimePeriod = " << TimePeriod << " ms" << endl;
    cout << "Numpoints = " << Npoints << endl;
    cout << "DataRange = " << dataRange << endl;
    cout << "data_generation_timing = " << dataGenerationTimingToString(dtg) << endl;
    cout << "filterType = " << filterTypeToString(Ft) << endl;
    cout << "--------------------------------------------------" << endl;
}
