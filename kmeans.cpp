#include <iostream>
#include <Windows.h>
#include <fstream>
#include <vector>
#include <cmath>
#include <limits>
#include <time.h>
#include <algorithm>
#include <string>
using namespace std;
void SetColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
void ResetColor() {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
}
void Intro() {
    SetColor(12);
    cout << "\t________________________________________________________________________________________________|\n";
    SetColor(12);
    cout << "\t|                     Truong: Dai Hoc Bach Khoa- Dai hoc Da Nang                                |\n";
    SetColor(12);
    cout << "\t|                            Khoa  : Cong nghe thong tin                                        |\n";
    SetColor(12);
    cout << "\t|                            PBL1  : DO AN LAP TRINH                                            |\n";
    SetColor(12);
    cout << "\t|          De tai: QUA TRINH TINH TOAN BANG THUAT TOAN PHAN CUM KMEANS                          |\n";
    SetColor(12);
    cout << "\t|                     Giao vien huong dan : Nguyen Tan Khoi                                     |\n";
    SetColor(12);
    cout << "\t|                     Sinh vien : Nguyen Thi Suong Mai - 23T_DT1                                |\n";
    SetColor(12);
    cout << "\t|                                 Tran Tam Nhu         - 23T_DT3                                |\n";
    SetColor(12);
    cout << "\t|_______________________________________________________________________________________________|\n";
    ResetColor();
}

class Point
{
private:
    int id_point, id_cluster;
    vector<double> values;
    int total_values;
    string name;
public:
    Point(int id_point, vector<double>& values, string name = "")
    {
        this->id_point = id_point;
        total_values = values.size();
        for(int i = 0; i < total_values; i++)
            this->values.push_back(values[i]);
        this->name = name;
        id_cluster = -1;
    }
    int getID()
    {
        return id_point;
    }
    void setCluster(int id_cluster)
    {
        this->id_cluster = id_cluster;
    }
    int getCluster()
    {
        return id_cluster;
    }
    double getValue(int index)
    {
        return values[index];
    }
    int getTotalValues()
    {
        return total_values;
    }
    string getName()
    {
        return name;
    }
};
class Cluster
{
private:
    int id_cluster;
    vector<double> central_values;
    vector<Point> points;
public:
    Cluster(int id_cluster, Point point)
    {
        this->id_cluster = id_cluster;
        int total_values = point.getTotalValues();
        for(int i = 0; i < total_values; i++)
            central_values.push_back(point.getValue(i));
        points.push_back(point);
    }
    void addPoint(Point point)
    {
        points.push_back(point);
    }
    bool removePoint(int id_point)
    {
        int total_points = points.size();
        for(int i = 0; i < total_points; i++)
        {
            if(points[i].getID() == id_point)
            {
                points.erase(points.begin() + i);
                return true;
            }
        }
        return false;
    }
    double getCentralValue(int index)
    {
        return central_values[index];
    }
    void setCentralValue(int index, double value)
    {
        central_values[index] = value;
    }
    Point getPoint(int index)
    {
        return points[index];
    }
    int getTotalPoints()
    {
        return points.size();
    }
    int getID()
    {
        return id_cluster;
    }
};
class KMeans
{
private:
    int K; // number of clusters
    int total_values, total_points, max_iterations;
    vector<Cluster> clusters;
    int getIDNearestCenter(Point point)
    {
        double sum = 0.0, min_dist;
        int id_cluster_center = 0;
        for(int i = 0; i < total_values; i++)
        {
            sum += pow(clusters[0].getCentralValue(i) - point.getValue(i), 2.0);
        }
        min_dist = sqrt(sum);
        for(int i = 1; i < K; i++)
        {
            double dist;
            sum = 0.0;
            for(int j = 0; j < total_values; j++)
            {
                sum += pow(clusters[i].getCentralValue(j) - point.getValue(j), 2.0);
            }
            dist = sqrt(sum);
            if(dist < min_dist)
            {
                min_dist = dist;
                id_cluster_center = i;
            }
        }
        return id_cluster_center;
    }
    Point findFarthestPoint(Cluster cluster)
{
    double max_dist = 0.0;
    Point farthest_point = cluster.getPoint(0);
    for (int i = 0; i < cluster.getTotalPoints(); i++)
    {
        double dist = 0.0;
        for (int j = 0; j < total_values; j++)
        {
            dist += pow(cluster.getPoint(i).getValue(j) - cluster.getCentralValue(j), 2.0);
        }
        dist = sqrt(dist);
        if (dist > max_dist)
        {
            max_dist = dist;
            farthest_point = cluster.getPoint(i);
        }
    }
    return farthest_point;
}
Point findClosestPoint(Cluster cluster)
{
    double min_dist =std :: numeric_limits<double>::max();
    Point closest_point = cluster.getPoint(0);
    for (int i = 0; i < cluster.getTotalPoints(); i++)
    {
        double dist = 0.0;
        for (int j = 0; j < total_values; j++)
        {
            dist += pow(cluster.getPoint(i).getValue(j) - cluster.getCentralValue(j), 2.0);
        }
        dist = sqrt(dist);
        if (dist < min_dist)
        {
            min_dist = dist;
            closest_point = cluster.getPoint(i);
        }
    }
    return closest_point;
}

public:
    KMeans(int K, int total_points, int total_values, int max_iterations)
    {
        this->K = K;
        this->total_points = total_points;
        this->total_values = total_values;
        this->max_iterations = max_iterations;
    }
    void run(vector<Point> & points)
    {
        if(K > total_points)
            return;
        vector<int> prohibited_indexes;
        int fixed_center_1 = 0; 
        points[fixed_center_1].setCluster(0);
        Cluster fixed_cluster_1(0, points[fixed_center_1]);
        clusters.push_back(fixed_cluster_1);
        int fixed_center_2 = 1; 
        points[fixed_center_2].setCluster(1);
        Cluster fixed_cluster_2(1, points[fixed_center_2]);
        clusters.push_back(fixed_cluster_2);
        for(int i = 2; i < K; i++)
        {
            while(true)
            {
                int index_point = rand() % total_points;
                if(find(prohibited_indexes.begin(), prohibited_indexes.end(), index_point) == prohibited_indexes.end())
                {
                    prohibited_indexes.push_back(index_point);
                    points[index_point].setCluster(i);
                    Cluster cluster(i, points[index_point]);
                    clusters.push_back(cluster);
                    break;
                }
            }
        }
        int iter = 1;
        while(true)
        {
            bool done = true;
            for(int i = 0; i < total_points; i++)
            {
                int id_old_cluster = points[i].getCluster();
                int id_nearest_center = getIDNearestCenter(points[i]);
                if(id_old_cluster != id_nearest_center)
                {
                    if(id_old_cluster != -1)
                        clusters[id_old_cluster].removePoint(points[i].getID());
                    points[i].setCluster(id_nearest_center);
                    clusters[id_nearest_center].addPoint(points[i]);
                    done = false;
                }
            }
            for(int i = 0; i < K; i++)
            {
                for(int j = 0; j < total_values; j++)
                {
                    int total_points_cluster = clusters[i].getTotalPoints();
                    double sum = 0.0;
                    if(total_points_cluster > 0)
                    {
                        for(int p = 0; p < total_points_cluster; p++)
                            sum += clusters[i].getPoint(p).getValue(j);
                        clusters[i].setCentralValue(j, sum / total_points_cluster);
                    }
                }
            }
            if(done == true || iter >= max_iterations)
            {
                cout << "End of iteration " << iter << "!!!\n\n";
                break;
            }
            iter++;
        }
        for (int i = 0; i < K; i++) {
        int total_points_cluster = clusters[i].getTotalPoints();
        cout << "Cluster " << clusters[i].getID() + 1 << endl;
        if (total_points_cluster > 0) {
            for (int j = 0; j < total_points_cluster; j++) {
               cout << "Point ID: " << clusters[i].getPoint(j).getID() + 1 << " Values: ";
               for (int k = 0; k < total_values; k++) {
                   cout << clusters[i].getPoint(j).getValue(k) << " ";
                }
               cout << endl;
            }

            double max_dist = numeric_limits<double>::min();
            double min_dist = numeric_limits<double>::max();
            Point farthest_point = clusters[i].getPoint(0);
            Point nearest_point = clusters[i].getPoint(0);
                  if (total_points_cluster > 1) {
                for (int j = 0; j < total_points_cluster; j++) {
                    Point current_point = clusters[i].getPoint(j);
                    double current_dist = 0.0;
                    for (int p = 0; p < total_values; p++) {
                        current_dist += pow(clusters[i].getCentralValue(p) - current_point.getValue(p), 2.0);
                    }
                    current_dist = sqrt(current_dist);
                    if (current_dist > max_dist) {
                        max_dist = current_dist;
                        farthest_point = current_point;
                    }
                    if (current_dist < min_dist && current_dist > 0) {
                        min_dist = current_dist;
                        nearest_point = current_point;
                    }
                }

            } else {            
                farthest_point = clusters[i].getPoint(0);
             nearest_point = clusters[i].getPoint(0);
                max_dist = 0.0;
                min_dist = 0.0;
            }
            cout << "Farthest Point: ID " << farthest_point.getID() + 1 << " Distance: " << max_dist << endl;
            cout << "Nearest Point: ID " << nearest_point.getID() + 1 << " Distance: " << min_dist << endl; 
        }
       
    cout << "Cluster values: ";
    for (int j = 0; j < total_values; j++)
        cout << clusters[i].getCentralValue(j) << " ";
    cout << endl << endl;
}
        } 
};
void getUserDefinedCenters(int total_values, int K, vector<Cluster>& clusters)
{
    for (int i = 0; i < K; i++)
    {
        cout << "Enter center values for Cluster " << i + 1 << ":" << endl;
        vector<double> center_values;
        for (int j = 0; j < total_values; j++)
        {
            double value;
            cout << "Enter value " << j + 1 << ": ";
            cin >> value;
            center_values.push_back(value);
        }
        Cluster cluster(i, Point(-1, center_values));
        clusters.push_back(cluster);
    }
}

int main()
{ 
Intro();

	int num_clusters;// nhap so luong tam cum tu nguoi su dung
	std:: cout << "Enter the number of clusters centers :";
	std:: cin >> num_clusters;
	if (num_clusters<= 0 || num_clusters >100){// kiem tra xem so luong tam cum co hop le khong
		std:: cout << " The number of clusters centers is impossible. Please enter again from 1 to 100 \n";
		return 1; // thoat chuong trinh voi ma loi
	}
		// xu ly so luong tam cum hop le
	std:: cout<< "The number of cluster centers entered is:  " << num_clusters<<std:: endl;
	 srand(time(NULL));
    int total_points, total_values, K, max_iterations, has_name;
    string filename;
cout << "Enter the name of file: ";
cin >> filename;
ifstream inputFile(filename);
if (!inputFile)
    {
        cerr << "Cannot open input file." << endl;
        return 1;
    }
    inputFile >> total_points >> total_values >> K >> max_iterations >> has_name;
    vector<Point> points;
    string point_name;
    for(int i = 0; i < total_points; i++)
    {
        vector<double> values;
        for(int j = 0; j < total_values; j++)
        {
            double value;
            inputFile >> value;
            values.push_back(value);
        }
        if(has_name)
        {
            inputFile >> point_name;
            Point p(i, values, point_name);
            points.push_back(p);
        }
        else
        {
            Point p(i, values);
            points.push_back(p);  
        }
    }
    vector<Cluster> clusters;
    int useDefinedCenters;
    cout << "Do you want to enter user-defined cluster centers? (1 for Yes, 0 for No): ";
    cin >> useDefinedCenters;
    if (useDefinedCenters == 1)
    {
        getUserDefinedCenters(total_values, K, clusters);
    }
    KMeans kmeans(K, total_points, total_values, max_iterations);
    kmeans.run(points);
    inputFile.close();
    return 0;
}

