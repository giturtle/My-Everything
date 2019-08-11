
#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <io.h>
#include <assert.h>
#include <windows.h>
#include <thread>
#include <mutex>
#include "./sqlite-amalgamation-3280000/sqlite3.h"

#define __DEBUG__
#define __TRACE__

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::set;
using std::vector;

//long _findfirst(char* filespec,struct _finddata_t *fileinfo);
	//失败返回-1，成功返回查找用的句柄，将在_findnext中被使用
//int _findnext(long handle,struct _finddata_t *fileinfo);
	//handle：_findfirst返回的句柄
	//fileinfo：文件信息结构体指针
static void DirectoryList(const string& path, vector<string>& dirs, vector<string>& files) {	//遍历到的目录通过引用返回
	//链接属性：static 只在当前文件可见
	_finddata_t file;
	const string actually_path = path + "\\*.*";	//目录下的所有文件

	//long _findfirst(const char *, struct _finddata_t *);
	intptr_t handle = _findfirst(actually_path.c_str(), &file);
	if (handle == -1) {
		cout << "_findfirst: " << path << "error" << endl;
		return;
	}
	do {
		cout << file.name << endl;
		if (file.attrib & _A_SUBDIR) {	//是文件夹(目录)
			if ((strcmp(file.name, ".") != 0) && (strcmp(file.name, "..") != 0)) {	//不等于.或者..再进入遍历
				dirs.push_back(file.name);
				/*
					string subpath = path;
					subpath += '\\';
					subpath += file.name;
					DirectoryList(subpath, docs);
				*/
			}
		}
		else {
			files.push_back(file.name);
		}
		//docs.push_back(file.name);
	} while (_findnext(handle, &file) == 0);	//成功则继续放入vector
	//int _findnext(long, struct _finddata_t *);
}



static void ColourPrintf(const char* str) {
	// 0-黑 1-蓝 2-绿 3-浅绿 4-红 5-紫 6-黄 7-白 8-灰 9-淡蓝 10-淡绿
	// 11-淡浅绿 12-淡红 13-淡紫 14-淡黄 15-亮白
	//颜色：前景色 + 背景色*0x10
	//例如：字是红色，背景色是白色，即 红色 + 亮白 = 4 + 15*0x10
	WORD color = 4 + 15 * 0x10;
	WORD colorOld;
	HANDLE handle = ::GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(handle, &csbi);
	colorOld = csbi.wAttributes;
	SetConsoleTextAttribute(handle, color);
	printf("%s", str);
	SetConsoleTextAttribute(handle, colorOld);
}

// 汉字转拼音全拼
static std::string ChineseConvertPinYinAllSpell(const std::string& dest_chinese) {
	static const int spell_value[] = { -20319, -20317, -20304, -20295, -20292, -20283,
   -20265, -20257, -20242, -20230, -20051, -20036, -20032, -20026,
	-20002, -19990, -19986, -19982, -19976, -19805, -19784, -19775, -19774, -19763,
   -19756, -19751, -19746, -19741, -19739, -19728,
	-19725, -19715, -19540, -19531, -19525, -19515, -19500, -19484, -19479, -19467,
   -19289, -19288, -19281, -19275, -19270, -19263,
	-19261, -19249, -19243, -19242, -19238, -19235, -19227, -19224, -19218, -19212,
   -19038, -19023, -19018, -19006, -19003, -18996,
	-18977, -18961, -18952, -18783, -18774, -18773, -18763, -18756, -18741, -18735,
   -18731, -18722, -18710, -18697, -18696, -18526,
	-18518, -18501, -18490, -18478, -18463, -18448, -18447, -18446, -18239, -18237,
   -18231, -18220, -18211, -18201, -18184, -18183,
	-18181, -18012, -17997, -17988, -17970, -17964, -17961, -17950, -17947, -17931,
   -17928, -17922, -17759, -17752, -17733, -17730,
	-17721, -17703, -17701, -17697, -17692, -17683, -17676, -17496, -17487, -17482,
   -17468, -17454, -17433, -17427, -17417, -17202,
	-17185, -16983, -16970, -16942, -16915, -16733, -16708, -16706, -16689, -16664,
   -16657, -16647, -16474, -16470, -16465, -16459,
	-16452, -16448, -16433, -16429, -16427, -16423, -16419, -16412, -16407, -16403,
   -16401, -16393, -16220, -16216, -16212, -16205,
	-16202, -16187, -16180, -16171, -16169, -16158, -16155, -15959, -15958, -15944,
   -15933, -15920, -15915, -15903, -15889, -15878,
	-15707, -15701, -15681, -15667, -15661, -15659, -15652, -15640, -15631, -15625,
   -15454, -15448, -15436, -15435, -15419, -15416,
	-15408, -15394, -15385, -15377, -15375, -15369, -15363, -15362, -15183, -15180,
   -15165, -15158, -15153, -15150, -15149, -15144,
	-15143, -15141, -15140, -15139, -15128, -15121, -15119, -15117, -15110, -15109,
   -14941, -14937, -14933, -14930, -14929, -14928,
	-14926, -14922, -14921, -14914, -14908, -14902, -14894, -14889, -14882, -14873,
   -14871, -14857, -14678, -14674, -14670, -14668,
	-14663, -14654, -14645, -14630, -14594, -14429, -14407, -14399, -14384, -14379,
   -14368, -14355, -14353, -14345, -14170, -14159,
	-14151, -14149, -14145, -14140, -14137, -14135, -14125, -14123, -14122, -14112,
   -14109, -14099, -14097, -14094, -14092, -14090,
	-14087, -14083, -13917, -13914, -13910, -13907, -13906, -13905, -13896, -13894,
   -13878, -13870, -13859, -13847, -13831, -13658,
	-13611, -13601, -13406, -13404, -13400, -13398, -13395, -13391, -13387, -13383,
   -13367, -13359, -13356, -13343, -13340, -13329,
	-13326, -13318, -13147, -13138, -13120, -13107, -13096, -13095, -13091, -13076,
   -13068, -13063, -13060, -12888, -12875, -12871,
	-12860, -12858, -12852, -12849, -12838, -12831, -12829, -12812, -12802, -12607,
	-12597, -12594, -12585, -12556, -12359, -12346,
	 -12320, -12300, -12120, -12099, -12089, -12074, -12067, -12058, -12039, -11867,
	-11861, -11847, -11831, -11798, -11781, -11604,
	 -11589, -11536, -11358, -11340, -11339, -11324, -11303, -11097, -11077, -11067,
	-11055, -11052, -11045, -11041, -11038, -11024,
	 -11020, -11019, -11018, -11014, -10838, -10832, -10815, -10800, -10790, -10780,
	-10764, -10587, -10544, -10533, -10519, -10331,
	 -10329, -10328, -10322, -10315, -10309, -10307, -10296, -10281, -10274, -10270,
	-10262, -10260, -10256, -10254 };

	// 395个字符串，每个字符串长度不超过6
	static const char spell_dict[396][7] = { "a", "ai", "an", "ang", "ao", "ba", "bai",
   "ban", "bang", "bao", "bei", "ben", "beng", "bi", "bian", "biao",
	"bie", "bin", "bing", "bo", "bu", "ca", "cai", "can", "cang", "cao", "ce", "ceng",
   "cha", "chai", "chan", "chang", "chao", "che", "chen",
	"cheng", "chi", "chong", "chou", "chu", "chuai", "chuan", "chuang", "chui", "chun",
   "chuo", "ci", "cong", "cou", "cu", "cuan", "cui",
	"cun", "cuo", "da", "dai", "dan", "dang", "dao", "de", "deng", "di", "dian",
   "diao", "die", "ding", "diu", "dong", "dou", "du", "duan",
	"dui", "dun", "duo", "e", "en", "er", "fa", "fan", "fang", "fei", "fen", "feng",
   "fo", "fou", "fu", "ga", "gai", "gan", "gang", "gao",
	"ge", "gei", "gen", "geng", "gong", "gou", "gu", "gua", "guai", "guan", "guang",
   "gui", "gun", "guo", "ha", "hai", "han", "hang",
	"hao", "he", "hei", "hen", "heng", "hong", "hou", "hu", "hua", "huai", "huan",
   "huang", "hui", "hun", "huo", "ji", "jia", "jian",
	"jiang", "jiao", "jie", "jin", "jing", "jiong", "jiu", "ju", "juan", "jue", "jun",
   "ka", "kai", "kan", "kang", "kao", "ke", "ken",
	"keng", "kong", "kou", "ku", "kua", "kuai", "kuan", "kuang", "kui", "kun", "kuo",
   "la", "lai", "lan", "lang", "lao", "le", "lei",
	"leng", "li", "lia", "lian", "liang", "liao", "lie", "lin", "ling", "liu", "long",
   "lou", "lu", "lv", "luan", "lue", "lun", "luo",
	"ma", "mai", "man", "mang", "mao", "me", "mei", "men", "meng", "mi", "mian",
   "miao", "mie", "min", "ming", "miu", "mo", "mou", "mu",
	"na", "nai", "nan", "nang", "nao", "ne", "nei", "nen", "neng", "ni", "nian",
   "niang", "niao", "nie", "nin", "ning", "niu", "nong",
	"nu", "nv", "nuan", "nue", "nuo", "o", "ou", "pa", "pai", "pan", "pang", "pao",
   "pei", "pen", "peng", "pi", "pian", "piao", "pie",
	"pin", "ping", "po", "pu", "qi", "qia", "qian", "qiang", "qiao", "qie", "qin",
   "qing", "qiong", "qiu", "qu", "quan", "que", "qun",
	"ran", "rang", "rao", "re", "ren", "reng", "ri", "rong", "rou", "ru", "ruan",
   "rui", "run", "ruo", "sa", "sai", "san", "sang",
	"sao", "se", "sen", "seng", "sha", "shai", "shan", "shang", "shao", "she", "shen",
   "sheng", "shi", "shou", "shu", "shua",
	"shuai", "shuan", "shuang", "shui", "shun", "shuo", "si", "song", "sou", "su",
   "suan", "sui", "sun", "suo", "ta", "tai",
	"tan", "tang", "tao", "te", "teng", "ti", "tian", "tiao", "tie", "ting", "tong",
   "tou", "tu", "tuan", "tui", "tun", "tuo",
	"wa", "wai", "wan", "wang", "wei", "wen", "weng", "wo", "wu", "xi", "xia", "xian",
   "xiang", "xiao", "xie", "xin", "xing",
	"xiong", "xiu", "xu", "xuan", "xue", "xun", "ya", "yan", "yang", "yao", "ye", "yi",
   "yin", "ying", "yo", "yong", "you",
	"yu", "yuan", "yue", "yun", "za", "zai", "zan", "zang", "zao", "ze", "zei", "zen",
		"zeng", "zha", "zhai", "zhan", "zhang",
 "zhao", "zhe", "zhen", "zheng", "zhi", "zhong", "zhou", "zhu", "zhua", "zhuai",
"zhuan", "zhuang", "zhui", "zhun", "zhuo",
 "zi", "zong", "zou", "zu", "zuan", "zui", "zun", "zuo" };

	std::string pinyin;
	// 循环处理字节数组
	const int length = dest_chinese.length();
	for (int j = 0, chrasc = 0; j < length;) {
		// 非汉字处理
		if (dest_chinese.at(j) >= 0 && dest_chinese.at(j) < 128) {
			pinyin += dest_chinese[j];
			// 偏移下标
			j++;
			continue;
		}
		// 汉字处理
		chrasc = dest_chinese[j] * 256 + dest_chinese[j + 1] + 256;
		if (chrasc > 0 && chrasc < 160) {
			// 非汉字
			pinyin += dest_chinese.at(j);
			// 偏移下标
			j++;
		}
		else {
			// 汉字
			for (int i = (sizeof(spell_value) / sizeof(spell_value[0]) - 1); i >= 0; --i) {
				// 查找字典
				if (spell_value[i] <= chrasc) {
					pinyin += spell_dict[i];
					break;
				}
			}
			// 偏移下标 （汉字双字节）
			j += 2;
		}
	} // for end
	return pinyin;
}


// 汉字转拼音首字母
static std::string ChineseConvertPinYinInitials(const std::string& name) {
	// 仅生成拼音首字母内容
	static int secPosValue[] = {
	1601, 1637, 1833, 2078, 2274, 2302, 2433, 2594, 2787, 3106, 3212,
	3472, 3635, 3722, 3730, 3858, 4027, 4086, 4390, 4558, 4684, 4925, 5249
	};
	static const char* initials[] = {
	"a", "b", "c", "d", "e", "f", "g", "h", "j", "k", "l", "m", "n", "o",
	 "p", "q", "r", "s", "t", "w", "x", "y", "z"
	};
	static const char* secondSecTable =
		"CJWGNSPGCGNE[Y[BTYYZDXYKYGT[JNNJQMBSGZSCYJSYY[PGKBZGY[YWJKGKLJYWKPJQHY[W[DZLSGMRYPYWWCCKZN \
		KYYGTTNJJNYKKZYTCJNMCYLQLYPYQFQRPZSLWBTGKJFYXJWZLTBNCXJJJJTXDTTSQZYCDXXHGCK[PHFFSS[YBGXLPPB \
		YLL[HLXS[ZM[JHSOJNGHDZQYKLGJHSGQZHXQGKEZZWYSCSCJXYEYXADZPMDSSMZJZQJYZC[J[WQJBYZPXGZNZCPWHKX \
		HQKMWFBPBYDTJZZKQHY"

		"LYGXFPTYJYYZPSZLFCHMQSHGMXXSXJ[[DCSBBQBEFSJYHXWGZKPYLQBGLDLCCTNMAYDDKSSNGYCSGXLYZAYBNPTSDK \
		DYLHGYMYLCXPY[JNDQJWXQXFYYFJLEJPZRXCCQWQQSBNKYMGPLBMJRQCFLNYMYQMSQYRBCJTHZTQFRXQHXMJJCJLXQG \
		JMSHZKBSWYEMYLTXFSYDSWLYCJQXSJNQBSCTYHBFTDCYZDJWYGHQFRXWCKQKXEBPTLPXJZSRMEBWHJLBJSLYYSMDXLC \
		LQKXLHXJRZJMFQHXHWY"

		"WSBHTRXXGLHQHFNM[YKLDYXZPYLGG[MTCFPAJJZYLJTYANJGBJPLQGDZYQYAXBKYSECJSZNSLYZHSXLZCGHPXZHZNY \
		TDSBCJKDLZAYFMYDLEBBGQYZKXGLDNDNYSKJSHDLYXBCGHXYPKDJMMZNGMMCLGWZSZXZJFZNMLZZTHCSYDBDLLSCDDN \
		LKJYKJSYCJLKWHQASDKNHCSGANHDAASHTCPLCPQYBSDMPJLPZJOQLCDHJJYSPRCHN[NNLHLYYQYHWZPTCZGWWMZFFJQ \
		QQQYXACLBHKDJXDGMMY"

		"DJXZLLSYGXGKJRYWZWYCLZMSSJZLDBYD[FCXYHLXCHYZJQ[[QAGMNYXPFRKSSBJLYXYSYGLNSCMHZWWMNZJJLXXHCH \
		SY[[TTXRYCYXBYHCSMXJSZNPWGPXXTAYBGAJCXLY[DCCWZOCWKCCSBNHCPDYZNFCYYTYCKXKYBSQKKYTQQXFCWCHCYK \
		ELZQBSQYJQCCLMTHSYWHMKTLKJLYCXWHEQQHTQH[PQ[QSCFYMNDMGBWHWLGSLLYSDLMLXPTHMJHWLJZYHZJXHTXJLHX \
		RSWLWZJCBXMHZQXSDZP"

		"MGFCSGLSXYMJSHXPJXWMYQKSMYPLRTHBXFTPMHYXLCHLHLZYLXGSSSSTCLSLDCLRPBHZHXYYFHB[GDMYCNQQWLQHJJ \
		[YWJZYEJJDHPBLQXTQKWHLCHQXAGTLXLJXMSL[HTZKZJECXJCJNMFBY[SFYWYBJZGNYSDZSQYRSLJPCLPWXSDWEJBJC \
		BCNAYTWGMPAPCLYQPCLZXSBNMSGGFNZJJBZSFZYNDXHPLQKZCZWALSBCCJX[YZGWKYPSGXFZFCDKHJGXDLQFSGDSLQW \
		ZKXTMHSBGZMJZRGLYJB"

		"PMLMSXLZJQQHZYJCZYDJWBMYKLDDPMJEGXYHYLXHLQYQHKYCWCJMYYXNATJHYCCXZPCQLBZWWYTWBQCMLPMYRJCCCX \
		FPZNZZLJPLXXYZTZLGDLDCKLYRZZGQTGJHHGJLJAXFGFJZSLCFDQZLCLGJDJCSNZLLJPJQDCCLCJXMYZFTSXGCGSBRZ \
		XJQQCTZHGYQTJQQLZXJYLYLBCYAMCSTYLPDJBYREGKLZYZHLYSZQLZNWCZCLLWJQJJJKDGJZOLBBZPPGLGHTGZXYGHZ \
		MYCNQSYCYHBHGXKAMTX"

		"YXNBSKYZZGJZLQJDFCJXDYGJQJJPMGWGJJJPKQSBGBMMCJSSCLPQPDXCDYYKY[CJDDYYGYWRHJRTGZNYQLDKLJSZZG \
		ZQZJGDYKSHPZMTLCPWNJAFYZDJCNMWESCYGLBTZCGMSSLLYXQSXSBSJSBBSGGHFJLYPMZJNLYYWDQSHZXTYYWHMZYHY \
		WDBXBTLMSYYYFSXJC[DXXLHJHF[SXZQHFZMZCZTQCXZXRTTDJHNNYZQQMNQDMMG[YDXMJGDHCDYZBFFALLZTDLTFXMX \
		QZDNGWQDBDCZJDXBZGS"

		"QQDDJCMBKZFFXMKDMDSYYSZCMLJDSYNSBRSKMKMPCKLGDBQTFZSWTFGGLYPLLJZHGJ[GYPZLTCSMCNBTJBQFKTHBYZ \
		GKPBBYMTDSSXTBNPDKLEYCJNYDDYKZDDHQHSDZSCTARLLTKZLGECLLKJLQJAQNBDKKGHPJTZQKSECSHALQFMMGJNLYJ \
		BBTMLYZXDCJPLDLPCQDHZYCBZSCZBZMSLJFLKRZJSNFRGJHXPDHYJYBZGDLQCSEZGXLBLGYXTWMABCHECMWYJYZLLJJ \
		YHLG[DJLSLYGKDZPZXJ"

		"YYZLWCXSZFGWYYDLYHCLJSCMBJHBLYZLYCBLYDPDQYSXQZBYTDKYXJY[CNRJMPDJGKLCLJBCTBJDDBBLBLCZQRPPXJ \
		CJLZCSHLTOLJNMDDDLNGKAQHQHJGYKHEZNMSHRP[QQJCHGMFPRXHJGDYCHGHLYRZQLCYQJNZSQTKQJYMSZSWLCFQQQX \
		YFGGYPTQWLMCRNFKKFSYYLQBMQAMMMYXCTPSHCPTXXZZSMPHPSHMCLMLDQFYQXSZYYDYJZZHQPDSZGLSTJBCKBXYQZJ \
		SGPSXQZQZRQTBDKYXZK"
		"HHGFLBCSMDLDGDZDBLZYYCXNNCSYBZBFGLZZXSWMSCCMQNJQSBDQSJTXXMBLTXZCLZSHZCXRQJGJYLXZFJPHYMZQQY \
		DFQJJLZZNZJCDGZYGCTXMZYSCTLKPHTXHTLBJXJLXSCDQXCBBTJFQZFSLTJBTKQBXXJJLJCHCZDBZJDCZJDCPRNPQCJ \
		PFCZLCLZXZDMXMPHJSGZGSZZQLYLWTJPFSYASMCJBTZKYCWMYTCSJJLJCQLWZMALBXYFBPNLSFHTGJWEJJXXGLLJSTG \
		SHJQLZFKCGNNNSZFDEQ"

		"FHBSAQTGYLBXMMYGSZLDYDQMJJRGBJTKGDHGKBLQKBDMBYLXWCXYTTYBKMRTJZXQJBHLMHMJJZMQASLDCYXYQDLQCA \
		FYWYXQHZ"; 

	const char* cName = name.c_str();

	std::string result;
	int H = 0, L = 0, W = 0, j = 0;
	size_t stringlen = ::strlen(cName);
	for (int i = 0; i < (int)stringlen; ++i) {
		H = (unsigned char)(cName[i + 0]);
		L = (unsigned char)(cName[i + 1]);
		if (H < 0xA1 || L < 0xA1) {
			result += cName[i];
			continue;
		}
		W = (H - 160) * 100 + L - 160;
		if (W > 1600 && W < 5590) {
			bool has = false;
			for (j = 22; j >= 0; j--) {
				if (W >= secPosValue[j]) {
					result += initials[j];
					i++;
					has = true;
					break;
				}
			}
			continue;
		}
		i++;
		W = (H - 160 - 56) * 94 + L - 161;
		if (W >= 0 && W <= 3007)
			result += secondSecTable[W];
		else {
			result += (unsigned char)H;
			result += (unsigned char)L;
		}
	}
	return result;
}

/////////////////////////////////////////////////
//	LOG

static std::string GetFileName(const std::string& path) {
	char ch = '/';
#ifdef _WIN32
	ch = '\\';
#endif
	size_t pos = path.rfind(ch);
	if (pos == std::string::npos)
		return path;
	else
		return path.substr(pos + 1);
}

//用于调试追溯的trace log
inline static void __TraceDebug(const char* filename, int line, const char* function, const char* format, ...) {
#ifdef __TRACE__
	//输出调用函数的信息
	fprintf(stdout, "[TRACE][%s:%d:%s]:", GetFileName(filename).c_str(), line, function);
	//输出用户打的trace信息
	va_list args;
	va_start(args, format);
	vfprintf(stdout, format, args);
	va_end(args);
	fprintf(stdout, "\n");
#endif
}

inline static void __ErrorDebug(const char* filename, int line, const char* function, const char* format, ...) {
#ifdef __DEBUG__
	//输出调用函数的信息
	fprintf(stdout, "[ERROR][%s:%d:%s]:", GetFileName(filename).c_str(), line, function);
	//输出用户打的trace信息
	va_list args;
	va_start(args, format);
	vfprintf(stdout, format, args);
	va_end(args);
	fprintf(stdout, " errmsg:%s, errno:%d\n", strerror(errno), errno);
#endif
}

#define TRACE_LOG(...) __TraceDebug(__FILE__, __LINE__, __FUNCTION__, __VA_ARGS__);
#define ERROE_LOG(...) __ErrorDebug(__FILE__, __LINE__, __FUNCTION__, __VA_ARGS__);
