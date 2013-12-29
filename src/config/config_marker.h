/*
 * config_marker.h
 *
 *  Created on: 29.12.2013
 *      Author: moritz
 */

#ifndef CONFIG_MARKER_H_
#define CONFIG_MARKER_H_

#include "extended_ini.h"
#include "validation.h"

#include "../mc/worldhelper.h"

namespace mapcrafter {
namespace config {

class MarkerSection {
public:
	MarkerSection(bool global = false);
	~MarkerSection();

	void setGlobal(bool global);
	bool parse(const ConfigSection& section, ValidationList& validation);

	std::string getPrefix() const;
	std::string getTitleFormat() const;
	std::string getTextFormat() const;
	std::string getIcon() const;
	std::string getIconSize() const;

	bool matchesSign(const mc::Sign& sign) const;
	std::string formatTitle(const mc::Sign& sign) const;
	std::string formatText(const mc::Sign& sign) const;
private:
	bool global;

	Field<std::string> prefix;
	Field<std::string> title_format, text_format;
	Field<std::string> icon, icon_size;

	std::string formatSign(std::string format, const mc::Sign& sign) const;
};

} /* namespace config */
} /* namespace mapcrafter */

#endif /* CONFIG_MARKER_H_ */
