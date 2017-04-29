#pragma once

#include <cstdio>
#include <string>
#include <iostream>
#include <sstream>

/*!
 * @author kovlev, sourcey.com (template)
 */


/*!
 * @struct Version
 * Handles version numbers and comparation
 */
struct Version {
	int major, minor, revision, build;

	Version(std::string version);

	bool operator< (const Version& other);
	bool operator== (const Version& other);
	bool operator!= (const Version& other);

	friend std::ostream& operator<< (std::ostream& out, Version const& ver);
	
	std::string toString();
};
