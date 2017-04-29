#pragma once

#include <iostream>
#include <cmath>

//Represents a 2 dimensional point with integer coordinates
class Point {
public:
	Point();
	Point(int newX, int newY);
	
	//Getters
	int getX();
	int getY();
	
	//Setters
	void setX(int newX);
	void setY(int newY);
	
	//Geometrical functions
	double vectorLength();
	double distanceTo(Point other);
	
	//Operator overloadnig in order to print out the point
	friend std::ostream& operator<<(std::ostream& out, Point const& point);
	
	//operator overloading for +- and negation
	Point operator+(const Point& other) const;
	Point operator-(const Point& other) const;
	Point operator-() const;
	
	//Multiplication and division works with integers
	Point operator*(const int& mul) const;
	Point operator/(const int& div) const;
	
	//'<operator> equal' overaloading
	Point operator+=(const Point& add);
	Point operator-=(const Point& sub);
	Point operator*=(const int& mul);
	Point operator/=(const int& div);
	
	//NOTE only for map comparission
	//Do not use this anywhere else
	bool operator<(const Point& other) const;
	
	//Helper function for comparission to check if it actually works;
	uint64_t hashCode();
	
	//These two are working properly
	bool operator==(const Point& other) const;
	bool operator!=(const Point& other) const;
private:
	int x;
	int y;
};

//Represents a 2 dimensional point with double precision coordinates
class PointD {
public:
	PointD();
	PointD(double newX, double newY);
	PointD(Point p);
	
	//Getters
	double getX();
	double getY();
	
	//Setters
	void setX(double newX);
	void setY(double newY);
	
	//Geometrical functions
	double vectorLength();
	double distanceTo(PointD other);
	
	//Operator overloadnig in order to print out the point
	friend std::ostream& operator<<(std::ostream& out, PointD const& point);
	
	//operator overloading for +- and negation
	PointD operator+(const PointD& other) const;
	PointD operator-(const PointD& other) const;
	PointD operator-() const;
	
	//Multiplication and division works with double values
	PointD operator*(const double& mul) const;
	PointD operator/(const double& div) const;
	
	//'<operator> equal' overaloading
	PointD operator+=(const PointD& add);
	PointD operator-=(const PointD& sub);
	PointD operator*=(const double& mul);
	PointD operator/=(const double& div);
	
	//These two are working properly
	bool operator==(const PointD& other) const;
	bool operator!=(const PointD& other) const;
private:
	double x;
	double y;
};
