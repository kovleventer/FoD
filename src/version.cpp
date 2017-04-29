#include "version.h"

Version::Version(std::string version) {
	std::sscanf(version.c_str(), "%d.%d.%d.%d", &major, &minor, &revision, &build);
	if (major < 0) major = 0;
	if (minor < 0) minor = 0;
	if (revision < 0) revision = 0;
	if (build < 0) build = 0;
}

bool Version::operator<(const Version& other) {
	if (major < other.major)
		return true;
	if (minor < other.minor)
		return true;
	if (revision < other.revision)
		return true;
	if (build < other.build)
		return true;
	return false;
}

bool Version::operator==(const Version& other) {
	return major == other.major 
		&& minor == other.minor 
		&& revision == other.revision 
		&& build == other.build;
}

bool Version::operator!=(const Version& other) {
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& out, Version const& ver) {
	out << ver.major;
	out << '.';
	out << ver.minor;
	out << '.';
	out << ver.revision;
	out << '.';
	out << ver.build;
	return out;
}

std::string Version::toString() {
	std::stringstream oss;
	oss << major;
	oss << '.';
	oss << minor;
	oss << '.';
	oss << revision;
	oss << '.';
	oss << build;
	return oss.str();
}
