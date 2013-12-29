/*
 * config_marker.cpp
 *
 *  Created on: 29.12.2013
 *      Author: moritz
 */

#include "config_marker.h"

#include "../util.h"

namespace mapcrafter {
namespace config {

MarkerSection::MarkerSection(bool global)
	: global(global) {
	title_format.setDefault("%text");
}

MarkerSection::~MarkerSection() {
}

std::string MarkerSection::formatSign(std::string format, const mc::Sign& sign) const {
	std::string textp = sign.getText();
	std::string text;
	if (textp.size() > prefix.getValue().size()) {
		text = textp.substr(prefix.getValue().size()+1);
		util::trim(text);
	}
	util::replaceAll(format, "%text", text);
	util::replaceAll(format, "%textp", textp);
	util::replaceAll(format, "%line0", sign.getLines()[0]);
	util::replaceAll(format, "%line1", sign.getLines()[1]);
	util::replaceAll(format, "%line2", sign.getLines()[2]);
	util::replaceAll(format, "%line3", sign.getLines()[3]);
	return format;
}

void MarkerSection::setGlobal(bool global) {
	this->global = global;
}

bool MarkerSection::parse(const ConfigSection& section, ValidationList& validation) {
	auto entries = section.getEntries();
	for (auto entry_it = entries.begin(); entry_it != entries.end(); ++entry_it) {
		std::string key = entry_it->first;
		std::string value = entry_it->second;

		if (key == "prefix")
			prefix.load(key, value, validation);
		else if (key == "title_format")
			title_format.load(key, value, validation);
		else if (key == "text_format")
			text_format.load(key, value, validation);
		else {
			validation.push_back(ValidationMessage::warning(
					"Unknown configuration option '" + key + "'!"));
		}
	}

	text_format.setDefault("%text");

	return true;
}

std::string MarkerSection::getPrefix() const {
	return prefix.getValue();
}

std::string MarkerSection::getTitleFormat() const {
	return title_format.getValue();
}

std::string MarkerSection::getTextFormat() const {
	return title_format.getValue();
}

std::string MarkerSection::getIcon() const {
	return icon.getValue();
}

bool MarkerSection::matchesSign(const mc::Sign& sign) const {
	return util::startswith(sign.getText(), prefix.getValue());
}

std::string MarkerSection::formatTitle(const mc::Sign& sign) const {
	return formatSign(title_format.getValue(), sign);
}

std::string MarkerSection::formatText(const mc::Sign& sign) const {
	return formatSign(text_format.getValue(), sign);
}

} /* namespace config */
} /* namespace mapcrafter */
