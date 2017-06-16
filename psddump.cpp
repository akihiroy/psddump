#include <stdio.h>
#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <array>
#include <vector>
#include <cctype>
#include <unordered_map>


int g_Indent = 0;

struct ScopedDumpSection {
	ScopedDumpSection(const std::string& name) {
		printf("%*s%s:\n", g_Indent * 2, "", name.c_str());
		g_Indent++;
	}
	ScopedDumpSection(const std::string& name, int64_t pos, uint64_t length) {
		printf("%*s%s (%016llx - %016llx %llubytes):\n", g_Indent * 2, "", name.c_str(), pos, pos + length, length);
		g_Indent++;
	}

	~ScopedDumpSection() {
		--g_Indent;
	}
};

void DumpInt(const char *name, int val, const char *comment = nullptr)
{
	if (comment) {
		printf("%*s%s: %d\t# %s\n", g_Indent * 2, "", name, val, comment);
	} else {
		printf("%*s%s: %d\n", g_Indent * 2, "", name, val);
	}
}

void DumpStr(const char *name, const char *val, int len = -1)
{
	std::string buf;
	if (len != -1) {
		buf.assign(val, len);
		val = buf.c_str();
	}
	printf("%*s%s: \"%s\"\n", g_Indent * 2, "", name, val);
}

template<size_t Size>
void DumpEnum(const char *name, int val, const std::array<const char *, Size>& table)
{
	printf("%*s%s: %d (%s)\n", g_Indent * 2, "", name, val, (0 <= val && val < Size) ? table[val] : "Unknown");
}

void DumpBin(const char *name, const uint8_t *data, size_t len, size_t max_len = -1)
{
	printf("%*s%s (%llubytes):\n", g_Indent * 2, "", name, (uint64_t)len);

	for (size_t i = 0; i < len && i < max_len; i += 16) {
		printf("%*s", g_Indent * 2 + 2, "");
		// Hex code
		for (size_t j = 0; j < 16; ++j) {
			if (i + j < len) {
				printf("%02x ", data[i + j]);
			} else {
				printf("   ");
			}
		}

		// ASCII
		for (size_t j = 0; j < 16; ++j) {
			if (i + j < len) {
				printf("%c", std::isprint(data[i + j]) ? data[i + j] : '.');
			}
		}
		printf("\n");
	}

	if (len > max_len) {
		printf("%*s...\n", g_Indent * 2 + 2, "");
	}
}

void DumpComment(const char *comment)
{
	printf("%*s# %s\n", g_Indent * 2, "", comment);
}


class PSDParser {
private:
	struct FileHeader {
		char signature[4];
		uint16_t version;
		uint16_t num_of_channels;
		uint32_t height;
		uint32_t width;
		uint16_t depth;
		uint16_t color_mode;
	};

	FileHeader m_Header;

	struct Layer {
		int32_t top, left, bottom, right;

		struct ChannelInfo {
			int16_t channel_id;
			uint64_t channel_data_len;

			const char *channel_comment() {
				if (channel_id == -1) return "\t# transparency mask";
				if (channel_id == -2) return "\t# user supplied layer mask";
				if (channel_id == -3) return "\t# real user supplied layer mask";
				return "";
			}
		};

		std::vector<ChannelInfo> channel_infos;
	};

	int16_t m_LayerCount;
	std::vector<Layer> m_Layers;

	std::shared_ptr<std::istream> m_Stream;


	template<class T> T read_integer() {
		if (m_Stream->eof()) {
			throw std::exception("Unexpected EOF");
		}
		T val;
		m_Stream->read((char *)&val, sizeof(T));
		std::reverse((uint8_t *)&val, ((uint8_t *)&val + sizeof(T)));	// Big endian -> Little endian
		return val;
	}

	std::string read_pascal_string(int align);

	void ReadResource1000();
	void ReadResource1001();
	void ReadResource1002();
	void ReadResource1003();
	void ReadResource1005();
	void ReadResource1006();
	void ReadResource1007();
	void ReadResource1008();
	void ReadResource1009();
	void ReadResource1010();
	void ReadResource1011();
	void ReadResource1012();
	void ReadResource1013();
	void ReadResource1014();
	void ReadResource1015();
	void ReadResource1016();
	void ReadResource1017();
	void ReadResource1018();
	void ReadResource1019();
	void ReadResource1020();
	void ReadResource1021();
	void ReadResource1022();
	void ReadResource1023();
	void ReadResource1024();
	void ReadResource1025();
	void ReadResource1026();
	void ReadResource1027();
	void ReadResource1028();
	void ReadResource1029();
	void ReadResource1030();
	void ReadResource1032();
	void ReadResource1033();
	void ReadResource1034();
	void ReadResource1035();
	void ReadResource1036();
	void ReadResource1037();
	void ReadResource1038();
	void ReadResource1039();
	void ReadResource1040();
	void ReadResource1041();
	void ReadResource1042();
	void ReadResource1043();
	void ReadResource1044();
	void ReadResource1045();
	void ReadResource1046();
	void ReadResource1047();
	void ReadResource1049();
	void ReadResource1050();
	void ReadResource1051();
	void ReadResource1052();
	void ReadResource1053();
	void ReadResource1054();
	void ReadResource1057();
	void ReadResource1058();
	void ReadResource1059();
	void ReadResource1060();
	void ReadResource1061();
	void ReadResource1062();
	void ReadResource1064();
	void ReadResource1065();
	void ReadResource1066();
	void ReadResource1067();
	void ReadResource1069();
	void ReadResource1070();
	void ReadResource1071();
	void ReadResource1072();
	void ReadResource1073();
	void ReadResource1074();
	void ReadResource1075();
	void ReadResource1076();
	void ReadResource1077();
	void ReadResource1078();
	void ReadResource1080();
	void ReadResource1082();
	void ReadResource1083();
	void ReadResource1084();
	void ReadResource1085();
	void ReadResource1086();
	void ReadResource1087();
	void ReadResource1088();
	void ReadResource2000();
	void ReadResource2999();
	void ReadResource3000();
	void ReadResource4000();
	void ReadResource7000();
	void ReadResource7001();
	void ReadResource7002();
	void ReadResource7003();
	void ReadResource7004();
	void ReadResource7005();
	void ReadResource7006();
	void ReadResource8000();
	void ReadResource10000();

	void ReadHeader();
	void ReadColorModeData();
	void ReadImageResources();
	void ReadLayerAndMaskInfo();
	void ReadLayers();
	void ReadImageData();

public:
	PSDParser(std::shared_ptr<std::istream> stream) : m_Stream(std::move(stream)) {}
	bool Dump();
};


std::string PSDParser::read_pascal_string(int align)
{
	if (m_Stream->eof()) {
		throw std::exception("Unexpected EOF");
	}
	uint8_t len;
	m_Stream->read((char *)&len, 1);

	std::string str;
	if (len > 0) {
		str.resize(len);
		m_Stream->read(&str[0], len);
	}

	// skip padding
	if ((1 + len) % align != 0) {
		m_Stream->seekg(align - (1 + len) % align, std::ios::cur);
	}

	return str;
}

void PSDParser::ReadResource1000()
{
	DumpComment("Number of channels, rows, columns, depth, and mode");
}

void PSDParser::ReadResource1001()
{
	DumpComment("Macintosh print manager print info record");
}

void PSDParser::ReadResource1002()
{
	DumpComment("Macintosh page format information");
}

void PSDParser::ReadResource1003()
{
	DumpComment("Indexed color table");
}

void PSDParser::ReadResource1005()
{
	DumpComment("ResolutionInfo");
}

void PSDParser::ReadResource1006()
{
	DumpComment("Names of the alpha channels");
}

void PSDParser::ReadResource1007()
{
	DumpComment("(Obsolete) See ID 1077DisplayInfo structure");
}

void PSDParser::ReadResource1008()
{
	DumpComment("The caption");
}

void PSDParser::ReadResource1009()
{
	DumpComment("Border information");
}

void PSDParser::ReadResource1010()
{
	DumpComment("Background color");
}

void PSDParser::ReadResource1011()
{
	DumpComment("Print flags");
}

void PSDParser::ReadResource1012()
{
	DumpComment("Grayscale and multichannel halftoning information");
}

void PSDParser::ReadResource1013()
{
	DumpComment("Color halftoning information");
}

void PSDParser::ReadResource1014()
{
	DumpComment("Duotone halftoning information");
}

void PSDParser::ReadResource1015()
{
	DumpComment("Grayscale and multichannel transfer function");
}

void PSDParser::ReadResource1016()
{
	DumpComment("Color transfer functions");
}

void PSDParser::ReadResource1017()
{
	DumpComment("Duotone transfer functions");
}

void PSDParser::ReadResource1018()
{
	DumpComment("Duotone image information");
}

void PSDParser::ReadResource1019()
{
	DumpComment("Two bytes for the effective black and white values for the dot range");
}

void PSDParser::ReadResource1020()
{
	DumpComment("Obsolete");
}

void PSDParser::ReadResource1021()
{
	DumpComment("EPS options");
}

void PSDParser::ReadResource1022()
{
	DumpComment("Quick Mask information");
}

void PSDParser::ReadResource1023()
{
	DumpComment("Obsolete");
}

void PSDParser::ReadResource1024()
{
	DumpComment("Layer state information");
}

void PSDParser::ReadResource1025()
{
	DumpComment("Working path");
}

void PSDParser::ReadResource1026()
{
	DumpComment("Layers group information");
}

void PSDParser::ReadResource1027()
{
	DumpComment("Obsolete");
}

void PSDParser::ReadResource1028()
{
	DumpComment("IPTC-NAA record");
}

void PSDParser::ReadResource1029()
{
	DumpComment("Image mode for raw format files");
}

void PSDParser::ReadResource1030()
{
	DumpComment("JPEG quality");
}

void PSDParser::ReadResource1032()
{
	DumpComment("Grid and guides information");
}

void PSDParser::ReadResource1033()
{
	DumpComment("Thumbnail resource");
}

void PSDParser::ReadResource1034()
{
	DumpComment("Copyright flag");
}

void PSDParser::ReadResource1035()
{
	DumpComment("URL");
}

void PSDParser::ReadResource1036()
{
	DumpComment("Thumbnail resource");
}

void PSDParser::ReadResource1037()
{
	DumpComment("Global Angle");
}

void PSDParser::ReadResource1038()
{
	DumpComment("Color samplers resource");
}

void PSDParser::ReadResource1039()
{
	DumpComment("ICC Profile");
}

void PSDParser::ReadResource1040()
{
	DumpComment("Watermark");
}

void PSDParser::ReadResource1041()
{
	DumpComment("ICC Untagged Profile");
}

void PSDParser::ReadResource1042()
{
	DumpComment("Effects visible");
}

void PSDParser::ReadResource1043()
{
	DumpComment("Spot Halftone");
}

void PSDParser::ReadResource1044()
{
	DumpComment("Document-specific IDs seed number");
}

void PSDParser::ReadResource1045()
{
	DumpComment("Unicode Alpha Names");
}

void PSDParser::ReadResource1046()
{
	DumpComment("Indexed Color Table Count");
}

void PSDParser::ReadResource1047()
{
	DumpComment("Transparency Index");
}

void PSDParser::ReadResource1049()
{
	DumpComment("Global Altitude");
}

void PSDParser::ReadResource1050()
{
	DumpComment("Slices");
}

void PSDParser::ReadResource1051()
{
	DumpComment("Workflow URL");
}

void PSDParser::ReadResource1052()
{
	DumpComment("Jump To XPEP");
}

void PSDParser::ReadResource1053()
{
	DumpComment("Alpha Identifiers");
}

void PSDParser::ReadResource1054()
{
	DumpComment("URL List");
}

void PSDParser::ReadResource1057()
{
	DumpComment("Version Info");
}

void PSDParser::ReadResource1058()
{
	DumpComment("EXIF data 1");
}

void PSDParser::ReadResource1059()
{
	DumpComment("EXIF data 3");
}

void PSDParser::ReadResource1060()
{
	DumpComment("XMP metadata");
}

void PSDParser::ReadResource1061()
{
	DumpComment("Caption digest");
}

void PSDParser::ReadResource1062()
{
	DumpComment("Print scale");
}

void PSDParser::ReadResource1064()
{
	DumpComment("Pixel Aspect Ratio");
}

void PSDParser::ReadResource1065()
{
	DumpComment("Layer Comps");
}

void PSDParser::ReadResource1066()
{
	DumpComment("Alternate Duotone Colors");
}

void PSDParser::ReadResource1067()
{
	DumpComment("Alternate Spot Colors");
}

void PSDParser::ReadResource1069()
{
	DumpComment("Layer Selection ID(s)");
}

void PSDParser::ReadResource1070()
{
	DumpComment("HDR Toning information");
}

void PSDParser::ReadResource1071()
{
	DumpComment("Print info");
}

void PSDParser::ReadResource1072()
{
	DumpComment("Layer Group(s) Enabled ID");
}

void PSDParser::ReadResource1073()
{
	DumpComment("Color samplers resource");
}

void PSDParser::ReadResource1074()
{
	DumpComment("Measurement Scale");
}

void PSDParser::ReadResource1075()
{
	DumpComment("Timeline Information");
}

void PSDParser::ReadResource1076()
{
	DumpComment("Sheet Disclosure");
}

void PSDParser::ReadResource1077()
{
	DumpComment("DisplayInfo structure");
}

void PSDParser::ReadResource1078()
{
	DumpComment("Onion Skins");
}

void PSDParser::ReadResource1080()
{
	DumpComment("Count Information");
}

void PSDParser::ReadResource1082()
{
	DumpComment("Print Information");
}

void PSDParser::ReadResource1083()
{
	DumpComment("Print Style");
}

void PSDParser::ReadResource1084()
{
	DumpComment("Macintosh NSPrintInfo");
}

void PSDParser::ReadResource1085()
{
	DumpComment("Windows DEVMODE");
}

void PSDParser::ReadResource1086()
{
	DumpComment("Auto Save File Path");
}

void PSDParser::ReadResource1087()
{
	DumpComment("Auto Save Format");
}

void PSDParser::ReadResource1088()
{
	DumpComment("Path Selection State");
}

void PSDParser::ReadResource2000()
{
	DumpComment("Path Information (saved paths)");
}

void PSDParser::ReadResource2999()
{
	DumpComment("Name of clipping path");
}

void PSDParser::ReadResource3000()
{
	DumpComment("Origin Path Info");
}

void PSDParser::ReadResource4000()
{
	DumpComment("Plug-In resource(s)");
}

void PSDParser::ReadResource7000()
{
	DumpComment("Image Ready variables");
}

void PSDParser::ReadResource7001()
{
	DumpComment("Image Ready data sets");
}

void PSDParser::ReadResource7002()
{
	DumpComment("Image Ready default selected state");
}

void PSDParser::ReadResource7003()
{
	DumpComment("Image Ready 7 rollover expanded state");
}

void PSDParser::ReadResource7004()
{
	DumpComment("Image Ready rollover expanded state");
}

void PSDParser::ReadResource7005()
{
	DumpComment("Image Ready save layer settings");
}

void PSDParser::ReadResource7006()
{
	DumpComment("Image Ready version");
}

void PSDParser::ReadResource8000()
{
	DumpComment("Lightroom workflow");
}

void PSDParser::ReadResource10000()
{
	DumpComment("Print flags information");
}

void PSDParser::ReadHeader()
{
	ScopedDumpSection section("FileHeader", 0, 26);

	m_Stream->read(m_Header.signature, 4);
	DumpStr("signature", m_Header.signature, 4);

	if (std::memcmp(m_Header.signature, "8BPS", 4) != 0) {
		throw std::exception("Error: Invalid signature");
	}

	m_Header.version = read_integer<uint16_t>();
	m_Stream->seekg(6, std::ios::cur);	// Reserved
	m_Header.num_of_channels = read_integer<uint16_t>();
	m_Header.height = read_integer<uint32_t>();
	m_Header.width = read_integer<uint32_t>();
	m_Header.depth = read_integer<uint16_t>();
	m_Header.color_mode = read_integer<uint16_t>();

	DumpInt("version", m_Header.version);
	DumpInt("num_of_channels", m_Header.num_of_channels);
	DumpInt("height", m_Header.height);
	DumpInt("width", m_Header.width);
	DumpInt("depth", m_Header.depth);

	const static std::array<const char *, 10> ColorMode = {"Bitmap", "Grayscale", "Indexed", "RGB", "CMYK", "Multichannel", "Duotone", "Lab"};
	DumpEnum("color_mode", m_Header.color_mode, ColorMode);
}

void PSDParser::ReadColorModeData()
{
	uint32_t length = read_integer<uint32_t>();
	ScopedDumpSection section("ColorModeData", m_Stream->tellg(), length);

	if (length > 0) {
		std::vector<uint8_t> color_data(length);
		m_Stream->read((char *)color_data.data(), length);
		DumpBin("color_data", color_data.data(), length, 16 * 4);
	}
}

void PSDParser::ReadImageResources()
{
	uint32_t length = read_integer<uint32_t>();
	auto begin = m_Stream->tellg();
	auto end = begin + (std::streamoff)length;

	ScopedDumpSection section("ImageResources", begin, length);

	while (*m_Stream && m_Stream->tellg() < end) {
		char signature[4];
		m_Stream->read(signature, 4);
		if (std::memcmp(m_Header.signature, "8BPS", 4) != 0) {
			throw std::exception("Error: Invalid signature");
		}

		uint16_t id = read_integer<uint16_t>();
		std::string name = read_pascal_string(2);
		uint32_t data_len = read_integer<uint32_t>();
		auto res_begin = m_Stream->tellg();
		auto res_end = res_begin + (std::streamoff)((data_len + 1) / 2 * 2);

		ScopedDumpSection section(std::string("Resource") + std::to_string(id), res_begin, data_len);

		if (id == 1000) ReadResource1000();
		else if (id == 1001) ReadResource1001();
		else if (id == 1002) ReadResource1002();
		else if (id == 1003) ReadResource1003();
		else if (id == 1005) ReadResource1005();
		else if (id == 1006) ReadResource1006();
		else if (id == 1007) ReadResource1007();
		else if (id == 1008) ReadResource1008();
		else if (id == 1009) ReadResource1009();
		else if (id == 1010) ReadResource1010();
		else if (id == 1011) ReadResource1011();
		else if (id == 1012) ReadResource1012();
		else if (id == 1013) ReadResource1013();
		else if (id == 1014) ReadResource1014();
		else if (id == 1015) ReadResource1015();
		else if (id == 1016) ReadResource1016();
		else if (id == 1017) ReadResource1017();
		else if (id == 1018) ReadResource1018();
		else if (id == 1019) ReadResource1019();
		else if (id == 1020) ReadResource1020();
		else if (id == 1021) ReadResource1021();
		else if (id == 1022) ReadResource1022();
		else if (id == 1023) ReadResource1023();
		else if (id == 1024) ReadResource1024();
		else if (id == 1025) ReadResource1025();
		else if (id == 1026) ReadResource1026();
		else if (id == 1027) ReadResource1027();
		else if (id == 1028) ReadResource1028();
		else if (id == 1029) ReadResource1029();
		else if (id == 1030) ReadResource1030();
		else if (id == 1032) ReadResource1032();
		else if (id == 1033) ReadResource1033();
		else if (id == 1034) ReadResource1034();
		else if (id == 1035) ReadResource1035();
		else if (id == 1036) ReadResource1036();
		else if (id == 1037) ReadResource1037();
		else if (id == 1038) ReadResource1038();
		else if (id == 1039) ReadResource1039();
		else if (id == 1040) ReadResource1040();
		else if (id == 1041) ReadResource1041();
		else if (id == 1042) ReadResource1042();
		else if (id == 1043) ReadResource1043();
		else if (id == 1044) ReadResource1044();
		else if (id == 1045) ReadResource1045();
		else if (id == 1046) ReadResource1046();
		else if (id == 1047) ReadResource1047();
		else if (id == 1049) ReadResource1049();
		else if (id == 1050) ReadResource1050();
		else if (id == 1051) ReadResource1051();
		else if (id == 1052) ReadResource1052();
		else if (id == 1053) ReadResource1053();
		else if (id == 1054) ReadResource1054();
		else if (id == 1057) ReadResource1057();
		else if (id == 1058) ReadResource1058();
		else if (id == 1059) ReadResource1059();
		else if (id == 1060) ReadResource1060();
		else if (id == 1061) ReadResource1061();
		else if (id == 1062) ReadResource1062();
		else if (id == 1064) ReadResource1064();
		else if (id == 1065) ReadResource1065();
		else if (id == 1066) ReadResource1066();
		else if (id == 1067) ReadResource1067();
		else if (id == 1069) ReadResource1069();
		else if (id == 1070) ReadResource1070();
		else if (id == 1071) ReadResource1071();
		else if (id == 1072) ReadResource1072();
		else if (id == 1073) ReadResource1073();
		else if (id == 1074) ReadResource1074();
		else if (id == 1075) ReadResource1075();
		else if (id == 1076) ReadResource1076();
		else if (id == 1077) ReadResource1077();
		else if (id == 1078) ReadResource1078();
		else if (id == 1080) ReadResource1080();
		else if (id == 1082) ReadResource1082();
		else if (id == 1083) ReadResource1083();
		else if (id == 1084) ReadResource1084();
		else if (id == 1085) ReadResource1085();
		else if (id == 1086) ReadResource1086();
		else if (id == 1087) ReadResource1087();
		else if (id == 1088) ReadResource1088();
		else if (2000 <= id && id <= 2997) ReadResource2000();
		else if (id == 2999) ReadResource2999();
		else if (id == 3000) ReadResource3000();
		else if (4000 <= id && id <= 4999) ReadResource4000();
		else if (id == 7000) ReadResource7000();
		else if (id == 7001) ReadResource7001();
		else if (id == 7002) ReadResource7002();
		else if (id == 7003) ReadResource7003();
		else if (id == 7004) ReadResource7004();
		else if (id == 7005) ReadResource7005();
		else if (id == 7006) ReadResource7006();
		else if (id == 8000) ReadResource8000();
		else if (id == 10000) ReadResource10000();
		else {
			DumpComment("Unknown resource");
		}

		m_Stream->seekg(res_end);
	}

	m_Stream->seekg(end);
}

void PSDParser::ReadLayerAndMaskInfo()
{
	uint64_t length = m_Header.version == 1 ? read_integer<uint32_t>() : read_integer<uint64_t>();
	auto begin = m_Stream->tellg();
	auto end = begin + (std::streamoff)length;

	ScopedDumpSection section("LayerAndMaskInfo", begin, length);

	if (length > 0) {
		ReadLayers();
	}

	m_Stream->seekg(end);
}

void PSDParser::ReadLayers()
{
	uint64_t length = m_Header.version == 1 ? read_integer<uint32_t>() : read_integer<uint64_t>();
	auto begin = m_Stream->tellg();
	auto end = begin + (std::streamoff)length;

	ScopedDumpSection section("LayerInfo", begin, length);

	if (length > 0) {
		m_LayerCount = read_integer<int16_t>();
		DumpInt("layer_count", m_LayerCount);

		unsigned int layer_count = std::abs(m_LayerCount);

		// Layer records
		for (unsigned int i = 0; i < layer_count; ++i) {
			ScopedDumpSection section(std::string("Layer") + std::to_string(i));

			Layer layer;
			layer.top = read_integer<int>();
			layer.left = read_integer<int>();
			layer.bottom = read_integer<int>();
			layer.right = read_integer<int>();
			uint16_t num_of_channels = read_integer<uint16_t>();
			layer.channel_infos.resize(num_of_channels);

			DumpInt("top", layer.top);
			DumpInt("left", layer.left);
			DumpInt("bottom", layer.bottom);
			DumpInt("right", layer.right);
			DumpInt("num_of_channels", num_of_channels);

			{
				ScopedDumpSection channel_info_section("channel_infos");
				for (size_t ch = 0; ch < num_of_channels; ++ch) {
					auto& channel_info = layer.channel_infos[ch];
					channel_info.channel_id = read_integer<int16_t>();
					channel_info.channel_data_len = m_Header.version == 1 ? read_integer<uint32_t>() : read_integer<uint64_t>();
					printf("%*s- {id: %d, data_length: %llu}%s\n", g_Indent * 2, "", channel_info.channel_id, channel_info.channel_data_len, channel_info.channel_comment());
				}
			}

			char blend_mode_signature[4];
			m_Stream->read(blend_mode_signature, 4);
			DumpStr("blend_mode_signature", blend_mode_signature, 4);
			if (std::memcmp(blend_mode_signature, "8BIM", 4) != 0) {
				throw std::exception("Error: Invalid blend mode signature");
			}

			char blend_mode_key[4];
			m_Stream->read(blend_mode_key, 4);
			DumpStr("blend_mode_key", blend_mode_key, 4);

			DumpInt("opacity", read_integer<uint8_t>());
			DumpInt("clipping", read_integer<uint8_t>());

			uint8_t flags = read_integer<uint8_t>();
			std::string flags_comment;
			if (flags & 1) flags_comment += "transparency protected, ";
			if (flags & 2) flags_comment += "visible, ";
			if (flags & 4) flags_comment += "obsolete, ";
			if (flags & 8) {
				flags_comment += "Photoshop 5.0 and later, ";
				if (flags & 16) flags_comment += "pixel data irrelevant to appearance of document, ";
			}
			if (!flags_comment.empty()) flags_comment.erase(flags_comment.size() - 2, 2);	// remove last comma
			DumpInt("flags", flags, flags_comment.c_str());

			m_Stream->seekg(1, std::ios::cur);	// Filler

			uint32_t extra_data_len = read_integer<uint32_t>();
			auto extra_data_begin = m_Stream->tellg();
			auto extra_data_end = extra_data_begin + (std::streamoff)extra_data_len;
			DumpInt("extra_data_len", extra_data_len);

			// Layer mask data

			// Layer blending ranges

			// Layer name

			m_Stream->seekg(extra_data_end);

			m_Layers.push_back(layer);
		}

		// Channel image data

	}

	m_Stream->seekg(end);
}

void PSDParser::ReadImageData()
{
}

bool PSDParser::Dump()
{
	ReadHeader();

	printf("\n");

	ReadColorModeData();

	printf("\n");

	ReadImageResources();

	printf("\n");

	ReadLayerAndMaskInfo();

	printf("\n");

	ReadImageData();

	return true;
}

int main(int argc, char *argv[])
{
	if (argc != 2) {
		printf("psddump [PSD file]\n");
		return 0;
	}

	try {
		auto stream = std::make_shared<std::ifstream>(argv[1], std::ios::in | std::ios::binary);
		if (!stream->good()) {
			throw std::exception("Cannot open file.");
		}
		PSDParser parser(std::move(stream));

		parser.Dump();
	}
	catch (const std::exception& ex) {
		fprintf(stderr, "Error: %s\n", ex.what());
	}

	return 0;
}
