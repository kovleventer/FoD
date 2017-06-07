#include "point.h"

/*!
 * @author kovlev
 */

//Point stuff

Point::Point() {
	x = 0;
	y = 0;
}

Point::Point(int newX, int newY) {
	x = newX;
	y = newY;
}

Point::Point(PointD p) {
	x = (int)p.getX();
	y = (int)p.getY();
}

int Point::getX() {
	return x;
}

int Point::getY() {
	return y;
}

void Point::setX(int newX) {
	x = newX;
}

void Point::setY(int newY) {
	y = newY;
}

double Point::vectorLength() {
	return sqrt(x * x + y * y);
}

double Point::distanceTo(Point other) {
	int dx = x - other.x;
	int dy = y - other.y;
	return sqrt(dx * dx + dy * dy);
}

int Point::distanceToManhattan(Point other) {
	int dx = std::abs(x - other.x);
	int dy = std::abs(y - other.y);
	return dx + dy;
}

int Point::distanceToChebyshev(Point other) {
	int dx = std::abs(x - other.x);
	int dy = std::abs(y - other.y);
	//Return the greater value as distance
	return dx > dy ? dx : dy;
}

std::ostream& operator<<(std::ostream& out, Point const& point) {
	out << "(" << point.x << "; " << point.y << ")";
	return out;
}

Point Point::operator+(const Point& other) const {
	Point p(x + other.x, y + other.y);
	return p;
}

Point Point::operator-(const Point& other) const {
	Point p(x - other.x, y - other.y);
	return p;
}

Point Point::operator-() const {
	Point p(-x, -y);
	return p;
}

Point Point::operator*(const int& mul) const {
	Point p(x * mul, y * mul);
	return p;
}

Point Point::operator/(const int& mul) const {
	Point p(x / mul, y / mul);
	return p;
}

Point Point::operator+=(const Point& add) {
	*this = *this + add;
	return *this;
}

Point Point::operator-=(const Point& sub) {
	*this = *this - sub;
	return *this;
}

Point Point::operator*=(const int& mul) {
	*this = *this * mul;
	return *this;
}

Point Point::operator/=(const int& div) {
	*this = *this / div;
	return *this;
}

bool Point::operator<(const Point& other) const {
	//Shifting magic
	uint64_t hashP1 = ((uint64_t)x) << 32 | y;
	uint64_t hashP2 = ((uint64_t)other.x) << 32 | other.y;
	return hashP1 < hashP2;
}

uint64_t Point::hashCode() {
	return ((uint64_t)x) << 32 | y;
}

bool Point::operator==(const Point& other) const {
	return x == other.x && y == other.y;
}

bool Point::operator!=(const Point& other) const {
	return !(*this == other);
}

const Point Point::INVALID = Point(-100, -100);


//Dimension stuff

Dimension::Dimension() : Dimension(0, 0) {}

Dimension::Dimension(int wp, int hp) {
	w = wp;
	h = hp;
}

int Dimension::W() {
	return w;
}

int Dimension::H() {
	return h;
}

Dimension Dimension::operator*=(const int& mul) {
	*this = *this * mul;
	return *this;
}

Dimension Dimension::operator/=(const int& div) {
	*this = *this / div;
	return *this;
}

std::ostream& operator<<(std::ostream& out, Dimension const& dimension) {
	out << "W: " << dimension.w << ", H: " << dimension.h;
	return out;
}

Dimension Dimension::operator*(const int& mul) const {
	Dimension d(w * mul, h * mul);
	return d;
}

Dimension Dimension::operator/(const int& mul) const {
	Dimension d(w / mul, h / mul);
	return d;
}


//PointD stuff

PointD::PointD() {
	x = 0;
	y = 0;
}

PointD::PointD(double newX, double newY) {
	x = newX;
	y = newY;
}

PointD::PointD(Point p) {
	x = p.getX();
	y = p.getY();
}

double PointD::getX() {
	return x;
}

double PointD::getY() {
	return y;
}

void PointD::setX(double newX) {
	x = newX;
}

void PointD::setY(double newY) {
	y = newY;
}

double PointD::vectorLength() {
	return sqrt(x * x + y * y);
}

double PointD::distanceTo(PointD other) {
	double dx = x - other.x;
	double dy = y - other.y;
	return sqrt(dx * dx + dy * dy);
}

double PointD::distanceToManhattan(PointD other) {
	double dx = std::abs(x - other.x);
	double dy = std::abs(y - other.y);
	return dx + dy;
}

double PointD::distanceToChebyshev(PointD other) {
	double dx = std::abs(x - other.x);
	double dy = std::abs(y - other.y);
	//Return the greater value as distance
	return dx > dy ? dx : dy;
}

std::ostream& operator<<(std::ostream& out, PointD const& point) {
	out.precision(2);
	out << "(" << point.x << "; " << point.y << ")";
	return out;
}

PointD PointD::operator+(const PointD& other) const {
	PointD p(x + other.x, y + other.y);
	return p;
}

PointD PointD::operator-(const PointD& other) const {
	PointD p(x - other.x, y - other.y);
	return p;
}

PointD PointD::operator-() const {
	PointD p(-x, -y);
	return p;
}

PointD PointD::operator*(const double& mul) const {
	PointD p(x * mul, y * mul);
	return p;
}

PointD PointD::operator/(const double& mul) const {
	PointD p(x / mul, y / mul);
	return p;
}

PointD PointD::operator+=(const PointD& add) {
	*this = *this + add;
	return *this;
}

PointD PointD::operator-=(const PointD& sub) {
	*this = *this - sub;
	return *this;
}

PointD PointD::operator*=(const double& mul) {
	*this = *this * mul;
	return *this;
}

PointD PointD::operator/=(const double& div) {
	*this = *this / div;
	return *this;
}

bool PointD::operator==(const PointD& other) const {
	return x == other.x && y == other.y;
}

bool PointD::operator!=(const PointD& other) const {
	return !(*this == other);
}

const PointD PointD::INVALID = PointD(-100, -100);
