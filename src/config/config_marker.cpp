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
}

MarkerSection::~MarkerSection() {

}

void MarkerSection::setGlobal(bool global) {
	this->global = global;
}

bool MarkerSection::parse(const ConfigSection& section, ValidationList& validation) {
	title_format.setDefault("%text");

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
	std::string title = title_format.getValue();
	util::replaceAll(title, "%text", sign.getText());
	util::replaceAll(title, "%line0", sign.getLines()[0]);
	util::replaceAll(title, "%line1", sign.getLines()[1]);
	util::replaceAll(title, "%line2", sign.getLines()[2]);
	util::replaceAll(title, "%line3", sign.getLines()[3]);
	return title;
}

std::string MarkerSection::formatText(const mc::Sign& sign) const {
	std::string text = text_format.getValue();
	util::replaceAll(text, "%text", sign.getText());
	util::replaceAll(text, "%line0", sign.getLines()[0]);
	util::replaceAll(text, "%line1", sign.getLines()[1]);
	util::replaceAll(text, "%line2", sign.getLines()[2]);
	util::replaceAll(text, "%line3", sign.getLines()[3]);
	return text;
}

} /* namespace config */
} /* namespace mapcrafter */
