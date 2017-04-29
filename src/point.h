#pragma once

#include <iostream>
#include <cmath>

//Needed becouse cross initialization
//Point(PointD p) and PointD(Point p)
class PointD;

//Represents a 2 dimensional point with integer coordinates
class Point {
public:
	Point();
	Point(int newX, int newY);
	//Casting double to int
	Point(PointD p);
	
	//Getters
	int getX();
	int getY();
	
	//Setters
	void setX(int newX);
	void setY(int newY);
	
	//Geometrical functions
	double vectorLength();
	double distanceTo(Point other);
	int distanceToManhattan(Point other);
	//Yes, this function is actually used
	//No, its not anymore
	int distanceToChebyshev(Point other);
	
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

//Same as point but the variable names make more sense
class Dimension {
public:
	Dimension(int wp, int hp);
	
	//Getters
	int W();
	int H();
	
	Dimension operator*=(const int& mul);
	Dimension operator/=(const int& div);
private:
	Dimension operator*(const int& mul) const;
	Dimension operator/(const int& div) const;
	
	int w, h;
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
	double distanceToManhattan(PointD other);
	double distanceToChebyshev(PointD other);
	
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
