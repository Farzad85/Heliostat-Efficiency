// Heliostat Efficiency.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <vector>
#include <iostream>
#include <math.h>
#include <stdio.h>




class Vector3
{
public:

    double X;
    double Y;
    double Z;

    Vector3(double* inputs)
    {
        X = inputs[0];
        Y = inputs[1];
        Z = inputs[2];
    }
    Vector3()
    {

    }
    double Dot(Vector3 other)
    {
        return X * other.X + Y * other.Y + Z * other.Z;
    }
    Vector3 Add(Vector3 other)

    {
        double arr[3] = { 0,0,0 };
        Vector3 result(arr);
        result.X = this->X + other.X;
        result.Y = this->Y + other.Y;
        result.Z = this->Z + other.Z;

        return result;
    }

    Vector3 Subtract(Vector3 other)
    {
        double arr[3] = { 0,0,0 };
        Vector3 result(arr);
        result.X = this->X - other.X;
        result.Y = this->Y - other.Y;
        result.Z = this->Z - other.Z;

        return result;
    }

    Vector3 Normalize()
    {
        double arr[3] = { 0,0,0 };
        Vector3 result(arr);
        result.X = X / sqrt(pow(X, 2) + pow(Y, 2) + pow(Z, 2));
        result.Y = Y / sqrt(pow(X, 2) + pow(Y, 2) + pow(Z, 2));
        result.Z = Z / sqrt(pow(X, 2) + pow(Y, 2) + pow(Z, 2));
        return result;
    }

    double Norm2()
    {
        return sqrt(pow(X, 2) + pow(Y, 2) + pow(Z, 2));
    }
    Vector3 ScalerDivide(double other)
    {
        double arr[3] = { 0,0,0 };
        Vector3 result(arr);
        result.X = X / other;
        result.Y = Y / other;
        result.Z = Z / other;
        return result;
    }
};



class ActuatedHeliostat
{
public:
    Vector3 location;
    Vector3 normal;

    ActuatedHeliostat(Vector3 Location, Vector3 Normal)
    {
        location = Location;
        normal = Normal;
    };

};

class HeliostatField
{
public:
    std::vector<ActuatedHeliostat> Heliostats;
public:
    void AddObj(ActuatedHeliostat obj)
    {
        this->Heliostats.push_back(obj);
    }
    void Track(Vector3 directionToSun, Vector3 targetLocation)
    {

        auto iter = Heliostats.begin();
        for (; iter != Heliostats.end(); iter++)
        {
            /// R is the unit vector pointing from a heliosat to the tower. 
            Vector3 R = (*iter).location.Subtract(targetLocation).Normalize();
            /// Updating Normal vector by using the sun direction vector and location of the heliosat. 
            (*iter).normal = R.Add(directionToSun).ScalerDivide(R.Add(directionToSun).Norm2());
        }  

    }

    double CosEfficiency(Vector3 directionToSun)
    {
        auto iter = Heliostats.begin();
        double projection = 0;
        int num = 0;
        for (; iter != Heliostats.end(); iter++)
        {
            projection += (*iter).normal.Dot(directionToSun);
            num += 1;
        }
        return projection / num;

    }
};

class OptimizableSpiralFieldGenerator
{
private:
    int ParameterCount;
    int get_ParameterCount()
    {
        return ParameterCount;
    };

public:
    HeliostatField Generate(int N, double a, double b)
    {


        if (N < 1)
        {
            throw std::invalid_argument("N should be a positive integer larger than 1.");
        };

        const int num = N;


        std::vector<ActuatedHeliostat> ActuatedHeliostatList;
        double r;
        double theta;
        HeliostatField field;
        for (int i = 1; i < N + 1; i++)
        {

            /// Constructing ActuatedHeliostat objects and computation of cartesian coordinates of each heliostat with respect to the tower location

            r = a * pow(i, b);
            theta = i * 137.508 * 2 * acos(0.0) / 180;
            double arr1[] = { r * cos(theta), r * sin(theta), 0 };
            Vector3 location(arr1);
            double arr2[] = { 0, 0, 1 };
            Vector3 normal(arr2);
            ActuatedHeliostat actuatedHeliostat(location, normal);
            field.AddObj(actuatedHeliostat);

        };
        return field;
    }
};


int main()
{
    std::cout << "Hello World!\n";

    {
        double temp1[3] = { 0.0, -0.6, 0.8 };
        Vector3 directionToSun(temp1);
        double temp2[3] = { 0, 0, 115 };
        Vector3 receiverLocation(temp2);
        int N = 624;
        double a = 4.5;
        double b = 0.65;
        OptimizableSpiralFieldGenerator generator;
        HeliostatField field;
        field = generator.Generate( N, a, b );
        field.Track(directionToSun, receiverLocation);
        double cosEfficiency = field.CosEfficiency(directionToSun);
        std::cout << "Cosine efficiency of the heliostat field is:       " << cosEfficiency<<'/n';
        std::cin.ignore();
    }

}



