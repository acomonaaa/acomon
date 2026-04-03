/**
 ****************************************************************************************************
 * @file        touch.c
 * @author      濠电姵顔栭崰妤冩崲閹邦喖绶ら柛褎顨呯壕濠氭煥濠靛棭妲搁柛灞诲姂閺岋綁寮崼顐ｎ棖闂佺妫勭粔褰掑蓟濞戙垹唯闁挎繂鎳愭导鍫濃攽?ALIENTEK) - 闂傚倸鍊风欢锟犲磻閸曨垰纾归柛褎顨呴崒銊╂煏閸繍妲归柛? * @version     V1.1
 * @date        2022-06-21
 * @brief       ATK-MD0280濠电姷顣藉Σ鍛村垂椤忓牆鐒垫い鎺戝暞閻濐亪鏌涚€ｃ劌鍔﹂柟顔煎槻閳诲骸鈻庨幘鎼偓宥夋⒑绾懏鐝紒顔肩Ч婵￠潧鈻庨幘宕囶攨闂佺粯鍔栭悾顐⑽旈埀顒勬箒闂佺粯鐟ラ幊鎰板箖婵傚憡鐓熼柡宥冨妿閻帡鏌℃担鍝バх€规洜鍘ч埞鎴﹀醇閻斿壊鍞撮梻鍌欒兌椤牏鎹㈤崼銉ョ閻忕偟鐡旈崵鏇熴亜閹烘垵顏柛銈傚亾闂備礁鎲￠崝蹇涘疾閳哄懎绀夐柍褜鍓熷铏圭磼濡搫顫庨梺纭呮珪閹搁箖寮鈧、鏇㈠Χ閸屾矮澹曞┑鐐村灦閻熴儲鎱ㄩ崶顒佺厽闁挎繂鎳岄崑銏⑩偓娈垮櫘閸撶喖寮婚崨顓涙婵☆垳绮▓銊╂⒒娴ｈ銇熼柛鐘虫礈濞嗐垽濡堕崶顭戞綗?
 * @note        濠电姷鏁搁崑娑⑺囬銏犵鐎广儱顦粈鍫澝归悡搴ｆ憼闁哄拋鍓氶幈銊ヮ潨閸℃瀛ｉ梺鍛婃煛閸嬫捇姊绘担铏瑰笡闁告梹顨婇獮濠囧箻閼稿灚娈板銈嗗姂閸婃劙宕戦幘鏂ユ婵炲棗绻愰～宀勬⒑娴兼瑧鍒伴柕鍫㈩焾椤曪綁顢楅崟顒傚姷濠电偞鍨惰摫闁哄棙娲熷鍝劽虹拠鍙夌亞闂佺粯甯粻鎾诲箚閳ь剟鏌涘☉娆愮稇閻庢艾顦埞鎴炲緞婵犲倸鐎璦sh闂備浇顕х€涒晝绮欓幒妤佹櫔闂備胶顭堥鍛矓閻熸壆鏆︽繝闈涱儐閸嬪嫰鎮归崶銊ョ祷妤犵偛顑夊娲箰鎼达絺妲堥梺鍝勭墱閸撶喖骞嗘笟鈧崹楣冨棘閵夛妇鈧姊洪悙钘夊姤閻忓繑鐟╅幃鐑藉箛閻楀牏鍘介梺闈涚箞閸ㄦ椽寮抽鍕厱闊洦妫戦懓鎸庝繆椤愶紕绐旈柛鈹惧亾濡炪倖甯掗崐褰掑疮閸濆嫨鈧帒顫濋敐鍛闂備線娼荤徊楣冨箖閸屾凹鍤曟い鎺戝閸嬪鏌涢鐘茬仼闁愁亙鍗冲? *              V1.1 婵犵數鍎戠徊钘壝归崒鐐茬獥闁哄稁鍘旈崶顒€钃熼柕澶涢檮濞呮牕鈹戦鏂や緵闁告鍋撻弲鍫曟晸閻樺磭鍘遍梺鐟扮摠缁诲啴藟閵忋倖鐓曢柨婵嗘噺閹叉悂鏌熸搴♀枅鐎殿喗鎸抽崺妤呭煛娴ｅ嘲顥氶梻浣告惈鐎氼剛鎹㈤幒鏃€鏆滈柛鈩冪⊕閻撳繘鏌涢埄鍐╃闁告柨绉垫穱濠囶敃閻樻祴鍋撳┑鍡╂綎濞寸姴顑呯粻娑欍亜閹哄秶顦﹂柛鎾跺厴濮婅櫣绱掑Ο鑲╃厾闂佹悶鍔岄悥鑹版＂闂佺懓澧界划顖炲煕閺冨牊鐓冮柣鐔稿濮樸劑鏌涚€ｎ偅灏扮紒瀣槺缁瑧绮堝鍥⒒婵犲骸浜滄繛璇х畵閹柉顦归柟顔芥そ楠炲鈹戦幇顒侇唫闂備礁鎲℃笟妤呭磻閳ь剟鏌涚€ｎ偅灏伴柟宄版嚇瀹曘劍绻濋崟顓涙瀸闂傚倷绀侀幖顐ょ矓閺夋嚚娲Χ閸稐姹楁繝銏ｅ煐閸旀洜绮婚崣澶堜簻妞ゆ劦鍋勯崢鎾煕鐎ｎ偅灏扮紒瀣槺閸栨牕鈻庢總鍛娾拺闁告繂瀚弳銏＄箾婢跺銆掗柤娲憾椤㈡﹢鎮╅悽绯曟敽? ****************************************************************************************************
 */

#include "touch.h"
#include "lcd.h"
#include "gpio.h"
#include <stdlib.h>
#include "cmsis_os.h"     /* [Fix3] FreeRTOS osDelay 闂傚倷鐒﹀鍨焽閸ф绀夐悗锝庡墲婵櫕銇勯幒鎴濐仼闁告劏鍋撻梻浣哄仺閸庨亶宕崹顔规瀺婵炲棙鍨圭壕鍏肩箾閹寸儑渚涙俊鎻掓憸缁?*/

/* 闂傚倷绀侀崥瀣磿閹惰棄搴婇柤鑹扮堪娴滃綊鏌?Y闂傚倷鑳堕～瀣礋椤愩埄娼旈梻浣虹帛閻楊剟寮搁崓涓嗛梻鍌欒兌椤牓鏁冮妸鈺佺；闁归偊鍏橀弸鏃堟煙鏉堝墽鐣辩€瑰憡绻堥弻鐔衡偓娑櫭粭姘攽?*/
#define TOUCH_CMD_X  0xD0
#define TOUCH_CMD_Y  0x90
#define TOUCH_CALIBRATION_MARGIN  20U

/* touch_get_adc2 闂傚倷绀侀幖顐︽偋閸愵喖纾婚柟鐐墯閻斿棝鏌涢銏☆棞婵炲眰鍊曢埢宥夊閵堝棗浠梺鎼炲劘閸斿秶绮堥崘鈹夸簻闁归偊鍏欓崑銏⑩偓娈垮櫘閸嬪棝寮鈧、娆撳礈瑜滃Σ鐑芥⒒閸屾艾鈧悂宕銏╁殨闁割偅娲﹂弫鍡樼箾閹存瑥鐏╃紒顐㈢Ч閺岋絽螣閼测晛绗″銈呭閻熝囧焵椤掑倹鏆柛瀣攻缁傚秹鎮欓搹鐟扮ウ?*/
/* [Fix] 婵?00婵犵數濮幏鍐川椤撴繄鎹曞┑鐘愁問閸犳岸宕戦妶鍛殾?00闂傚倷鐒︾€笛呯矙閹达附鍎旈柤娴嬫櫅閸ㄦ繈鏌ｅΔ鈧悧蹇涖€呴弻銉︾厱妞ゆ劗鍠愰崳浼存煃瑜滈崜姘舵偋濡ゅ啰鐭夐柟鐑橆殕閸ゅ鏌涢…鎴濅簽闁告垼濮ょ换婵嬪閿濆懐鍘┑顔角滈崝蹇氥亹婵犲洦鐓熼幖娣€ゅ鎰棯閺夎法孝妞ゎ亜鍟村畷鎺楁倷缁瀚?*/
#define TOUCH_ADC2_MAX_RETRY  500

/* ATK-MD0280濠电姷顣藉Σ鍛村垂椤忓牆鐒垫い鎺戝暞閻濐亪鏌涚€ｃ劌鍔﹂柟顔煎槻閳诲骸鈻庨幘鎼偓宥夋⒑绾懏鐝紒顔芥崌楠炲啴鍩￠崨顔兼疂缂佸墽澧楅敋濞存粍鍎抽湁闁挎繂鐗滃鎰箾閸繄鍩ｉ柡宀€鍠撻幏鐘诲灳閾忣偆浜剧紓鍌欑贰閸犳捇宕濋幋婵愬殨闁归棿绀佺粈瀣亜韫囨挻顥犲?*/
/* Touch calibration state */
static struct
{
    struct
    {
        float x;
        float y;
    } fac;
    struct
    {
        uint16_t x;
        uint16_t y;
    } center;
} g_touch_sta = {0};

/* [Fix7] SPI 闂佽娴烽崑锝夊磹濞戙垹鏄ラ柡宓本瀵岄梺绋跨灱閸嬬偤寮查浣瑰弿婵妫楁禍婊堟煃鐠囪尙绠婚柡?__NOP() 闂傚倷绀侀幖顐⒚洪妶澶娢︽慨妞诲亾鐎?volatile 缂傚倸鍊风粈渚€鎳熼鐐茬妞ゆ劧绠戠粻姘攽閻樺弶澶勯柛瀣箖閵囧嫰寮介妷銉㈠亾濡ゅ啰鐭嗗鑸靛姈閻撴洟鏌ㄩ弬鎸庢儓濠德ゆ缁辨帒螖閳ь剟藝闂堟稓鏆﹂柕澹倹鍕冮梺鍏肩ゴ閺呮盯寮搁悩缁樷拺闁告稑锕︽晶顒勬煟濡や胶鐭嬫い銊ｅ劦瀹曞爼顢楅埀顒傜矆閸℃ǜ浜滈柟鏉垮绾惧潡鏌ｉ悢鍛婂碍閼挎劙鏌涢妷锝呭缂佺姵鐗犻幃妤呭垂椤愶絺鎷圭紓浣割儏椤︾敻宕洪埀顒併亜閹烘垵顏柛?*/
static inline void touch_spi_delay_short(void)
{
    __NOP(); __NOP(); __NOP(); __NOP();
}

static inline void touch_spi_delay_long(void)
{
    __NOP(); __NOP(); __NOP(); __NOP();
    __NOP(); __NOP(); __NOP(); __NOP();
    __NOP(); __NOP(); __NOP(); __NOP();
    __NOP(); __NOP(); __NOP(); __NOP();
    __NOP(); __NOP(); __NOP(); __NOP();
    __NOP(); __NOP(); __NOP(); __NOP();
}

/**
 * @brief       ATK-MD0280濠电姷顣藉Σ鍛村垂椤忓牆鐒垫い鎺戝暞閻濐亪鏌涚€ｃ劌鍔﹂柟顔煎槻閳诲骸鈻庨幘鎼偓宥夋⒑绾懏鐝紒顔奸閳藉鎮介崜鍙夋櫈闂佸湱顭堢€垫帡宕戦妸鈺傗拺闁告稑锕ょ粭姘辩磼鐠囨彃鈧潡鏁愰悙鍝勭疀闁哄娉曢ˇ? */
static void touch_hw_init(void)
{
    GPIO_InitTypeDef gpio_init_struct = {0};
    
    /* 婵犵數鍋犻幓顏嗙礊閳ь剟鏌涙惔鈽嗙吋鐎规洘顨呴～婊堝焵椤掆偓椤曪綁顢氶埀顒€鐣锋總绋垮嵆闁绘柨顨庨崵?*/
    TOUCH_PEN_GPIO_CLK_ENABLE();
    
    /* 闂傚倷绀侀幉锛勬暜濡ゅ啯宕查柛宀€鍎戠紞鏍煙閻楀牊绶茬紒鈧畝鍕厸闁割偁鍨瑰▓娣ｉ梺璇插椤旀牠宕板Δ鈧叅婵犻潧顑呯粈?- 婵犵數鍋為崹鍫曞箰閹间焦鏅濋柕澶涚畱閸ㄦ棃鏌ｉ幋鐐嗘垹绱為崶顒佺厵闁诡垎鍐╂瘣閻庢鍠楁繛濠囧蓟閵娿儮妲堟俊顖滅帛閹烽亶鏌ｆ惔锛勪粵缂侇喖绉剁划娆愬緞鐏炵浜鹃柨婵嗛娴滄繃銇勯敂缁樼EN=0 */
    gpio_init_struct.Pin    = TOUCH_PEN_GPIO_PIN;
    gpio_init_struct.Mode   = GPIO_MODE_INPUT;
    gpio_init_struct.Pull   = GPIO_PULLUP;
    gpio_init_struct.Speed  = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(TOUCH_PEN_GPIO_PORT, &gpio_init_struct);
}

/**
 * @brief       ATK-MD0280濠电姷顣藉Σ鍛村垂椤忓牆鐒垫い鎺戝暞閻濐亪鏌涚€ｃ劌鍔﹂柟顔煎槻閳诲骸鈻庨幘鎼偓宥夋⒑绾懏鐝柦妯稿及I闂傚倷娴囬～澶嬬娴犲纾块弶鍫亖娴滆绻涢幋娆忕仼缂佲偓閸愨晝绠鹃柛鈩兠慨鍫熺箾閸繄鍩ｉ柡? * @param       dat: 闂佽楠搁悘姘熆濮椻偓楠炲﹥鎯旈妸銉ф焾闂佽宕橀褏绮堥崒鐐寸厪濠㈣鍨扮€氼噣寮抽悩缁樷拺閺夌偞濯介崗灞筋熆瑜庨〃鍛存偩?
 */
static void touch_spi_write(uint8_t dat)
{
    uint8_t dat_index;
    
    for (dat_index = 0; dat_index < 8; dat_index++)
    {
        TOUCH_SPI_CLK(0);
        if (dat & 0x80)
        {
            TOUCH_SPI_MO(1);
        }
        else
        {
            TOUCH_SPI_MO(0);
        }
        dat <<= 1;
        TOUCH_SPI_CLK(1);
    }
    
    TOUCH_SPI_CLK(1);
    TOUCH_SPI_CLK(0);
}

/**
 * @brief       ATK-MD0280濠电姷顣藉Σ鍛村垂椤忓牆鐒垫い鎺戝暞閻濐亪鏌涚€ｃ劌鍔﹂柟顔煎槻閳诲骸鈻庨幘鎼偓宥夋⒑绾懏鐝柦妯稿及I闂傚倷娴囬～澶嬬娴犲纾块弶鍫亖娴滆绻涢幋鐐殿暡閻庢碍姘ㄧ槐鎾存媴閼测剝鍨甸埢鎾诲醇閺囩喓鍘? * @param       cmd: 闂備浇宕垫慨鏉懨洪埡鍜佹晪鐟滄垿濡甸幇鏉跨倞妞ゆ巻鍋撶€瑰憡绻堥弻鐔衡偓娑櫭粭姘攽?
 * @retval      闂備浇宕垫慨鏉懨洪埡鍜佹晪鐟滄柨鐣烽悽鍓叉晣闁靛繆鈧啿澹?2婵犵數鍋犻幓顏嗗緤閼恒儱顕遍柣銈呰埗闂傚倷娴囧銊╂嚄閼稿灚娅犳俊銈傚亾闁?
 */
static uint16_t touch_spi_read(uint8_t cmd)
{
    uint16_t dat = 0;
    uint8_t dat_index;
    
    TOUCH_SPI_CLK(0);
    TOUCH_SPI_MO(0);
    TOUCH_SPI_TCS(0);
    touch_spi_write(cmd);
    
    /* [Fix7] 婵犵數鍋犻幓顏嗙礊閳ь剚绻涙径瀣鐎?__NOP() 闂佽娴烽崑锝夊磹濞戙垹鏄ラ柡宓本瀵岄梺绋跨灱閸嬬偤寮查鍕€堕柣鎰版涧娴滃墽绱掗崒姘冲闂?volatile 闂佽娴烽弫濠氬磻婵犲啰顩查柣鎰瀹?*/
    touch_spi_delay_long();
    
    TOUCH_SPI_CLK(0);
    touch_spi_delay_short();
    TOUCH_SPI_CLK(1);
    touch_spi_delay_short();
    TOUCH_SPI_CLK(0);
    
    for (dat_index = 0; dat_index < 16; dat_index++)
    {
        dat <<= 1;
        TOUCH_SPI_CLK(0);
        touch_spi_delay_short();
        TOUCH_SPI_CLK(1);
        if (TOUCH_SPI_READ_MI() != 0)
        {
            dat++;
        }
    }
    
    dat >>= 4;  /* 闂傚倷绀侀幉锟犳偡椤栨稓顩叉繝濠傛娴滃綊鏌涢妷顔惧帥婵?2婵?*/
    TOUCH_SPI_TCS(1);
    
    return dat;
}

/**
 * @brief       ATK-MD0280濠电姷顣藉Σ鍛村垂椤忓牆鐒垫い鎺戝暞閻濐亪鏌涚€ｃ劌鍔﹂柟顔煎槻閳诲骸鈻庨幘鎼偓宥夋⒑绾懏鐝柦妯稿及I闂傚倷娴囬～澶嬬娴犲纾块弶鍫亖娴滆绻涢幋娆忕仼缂佲偓閸℃绡€闂傚牊绋掗敍宥嗙箾閸忕⒈娈滈柡? */
static void touch_spi_init(void)
{
    GPIO_InitTypeDef gpio_init_struct = {0};
    
    /* 婵犵數鍋犻幓顏嗙礊閳ь剟鏌涙惔鈽嗙吋鐎规洘顨呴～婊堝焵椤掆偓椤曪綁顢氶埀顒€鐣锋總绋垮嵆闁绘柨顨庨崵?*/
    TOUCH_SPI_MI_GPIO_CLK_ENABLE();
    TOUCH_SPI_MO_GPIO_CLK_ENABLE();
    TOUCH_SPI_TCS_GPIO_CLK_ENABLE();
    TOUCH_SPI_CLK_GPIO_CLK_ENABLE();
    
    /* 闂傚倷绀侀幉锛勬暜濡ゅ啯宕查柛宀€鍎戠紞鏍煙閻楀牊绶茬紒鈧畝鍕厸闁割偆鍣ラ崕锝夋煟鎼淬埄鍟忛柛鐘愁殔鐓ゆ繝闈涱儏缁€?- 婵犵數鍋為崹鍫曞箰閹间焦鏅濋柕澶涚畱閸ㄦ棃鏌ｉ幋鐐嗘垹绱為崶顒佺厵闁诡垎鍐╂瘣閻?*/
    gpio_init_struct.Pin    = TOUCH_SPI_MI_GPIO_PIN;
    gpio_init_struct.Mode   = GPIO_MODE_INPUT;
    gpio_init_struct.Pull   = GPIO_PULLUP;
    gpio_init_struct.Speed  = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(TOUCH_SPI_MI_GPIO_PORT, &gpio_init_struct);
    
    /* 闂傚倷绀侀幉锛勬暜濡ゅ啯宕查柛宀€鍎戠紞鏍煙閻楀牊绶茬紒鈧畝鍕厸闁割偆鍠撻幖鍫曟煟鎼淬埄鍟忛柛鐘愁殔鐓ゆ繝闈涱儏缁€?*/
    gpio_init_struct.Pin    = TOUCH_SPI_MO_GPIO_PIN;
    gpio_init_struct.Mode   = GPIO_MODE_OUTPUT_PP;
    gpio_init_struct.Pull   = GPIO_PULLUP;
    gpio_init_struct.Speed  = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(TOUCH_SPI_MO_GPIO_PORT, &gpio_init_struct);
    
    /* 闂傚倷绀侀幉锛勬暜濡ゅ啯宕查柛宀€鍎戠紞鏍煙閻楀牊绶茬紒鈧畝鍕厸闁割偒鍋呴敍宄夐梺璇插椤旀牠宕板Δ鈧叅婵犻潧顑呯粈?*/
    gpio_init_struct.Pin    = TOUCH_SPI_TCS_GPIO_PIN;
    gpio_init_struct.Mode   = GPIO_MODE_OUTPUT_PP;
    gpio_init_struct.Pull   = GPIO_PULLUP;
    gpio_init_struct.Speed  = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(TOUCH_SPI_TCS_GPIO_PORT, &gpio_init_struct);
    TOUCH_SPI_TCS(1);  /* 闂傚倷鑳剁划顖炪€冮崨顒兼椽顢橀姀鐘靛姦濡炪倖宸婚崑鎾淬亜閿旂偓鏆╅柛鎺撳浮椤㈡稑顭ㄩ崟顐偓娑㈡⒑闂堟稓澧曟い锕佺簿閵囨劙顢涢悙瀵稿幗闂侀潧绻堥崐鏍р槈瑜斿?*/
    
    /* 闂傚倷绀侀幉锛勬暜濡ゅ啯宕查柛宀€鍎戠紞鏍煙閻楀牊绶茬紒鈧畝鍕厸闁割偅绻冮～顡戦梺璇插椤旀牠宕板Δ鈧叅婵犻潧顑呯粈?*/
    gpio_init_struct.Pin    = TOUCH_SPI_CLK_GPIO_PIN;
    gpio_init_struct.Mode   = GPIO_MODE_OUTPUT_PP;
    gpio_init_struct.Pull   = GPIO_PULLUP;
    gpio_init_struct.Speed  = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(TOUCH_SPI_CLK_GPIO_PORT, &gpio_init_struct);
    TOUCH_SPI_CLK(1);  /* CLK婵犳鍠楃敮妤冪矙閹烘せ鈧箓宕奸妷顔芥櫍缂傚倷鐒﹂敃鈺佲枔濡や椒绻嗘い鏍ㄨ壘閹垿鏌￠崱蹇旀珕妞?*/
}

/**
 * @brief       闂傚倷绀侀崥瀣磿閹惰棄搴婇柤鑹扮堪娴滃綊鏌涢妷銏℃珕闁哥姴妫濋弻銊╁籍閸ヮ灝鎾绘煕閵堝拋鍎旈柡宀嬬秮婵℃悂濡烽妷褌绨?Y闂備礁鎼ˇ閬嶅磻閻旂鈧倿鎮·澶愭⒒娴ｄ警鐒鹃柨鏇畵婵″鈧綆鍓氶～鏇熺箾閸℃ê濮囩紒鍓佸仦閹便劌顫滈崱妤€瀛ｉ梺绋跨應閸ャ劉鎷洪梺鍦帛鐢寰勯崟顖涚厽婵°倕鎳嶉幉鍓р偓瑙勬穿缂嶄礁顕ｉ崐鐕佹▌闂佺懓澹婇崜鐔煎蓟濞戞粠妲诲┑鐐存綑鐎氼喖危?
 * @param       cmd: TOUCH_CMD_X 闂?TOUCH_CMD_Y
 * @retval      闂傚倷绀侀崥瀣磿閹惰棄搴婇柤鑹扮堪娴滃綊鏌涢妷顔煎缂佲偓閸℃稒鐓欐繛鍫濈仢閺嬬喖鏌ｉ敐澶夋喚闁诡喖鍢查埢搴♀枎閹炬惌鈧秹姊虹涵鍛彧缂佸瞼绋楥闂? */
static uint16_t touch_get_adc(uint8_t cmd)
{
    uint16_t dat[TOUCH_READ_TIMES];
    uint8_t dat_index;
    uint8_t dat_index2;
    uint16_t dat_tmp;
    uint16_t dat_sum = 0;
    
    /* 闂備礁鎼ˇ顐﹀疾濠靛纾婚柣鏂垮悑閸嬨劑鏌ゆ慨鎰偓鏍偓姘哺閹綊宕堕妸銉хシ缂備焦褰冮悘婵嬧€﹂崸妤佸殝闁汇垺顔栭弳鈥愁渻?*/
    for (dat_index = 0; dat_index < TOUCH_READ_TIMES; dat_index++)
    {
        dat[dat_index] = touch_spi_read(cmd);
    }
    
    /* 闂傚倷绀侀幉锟犲礉閺嶎偅鏆滄俊銈呮噹閹硅埖绻涘顔荤盎缂佺姵濞婇弻鐔兼倻濡櫣浠肩紒?*/
    for (dat_index = 0; dat_index < (TOUCH_READ_TIMES - 1); dat_index++)
    {
        for (dat_index2 = (dat_index + 1); dat_index2 < TOUCH_READ_TIMES; dat_index2++)
        {
            if (dat[dat_index] > dat[dat_index2])
            {
                dat_tmp = dat[dat_index];
                dat[dat_index] = dat[dat_index2];
                dat[dat_index2] = dat_tmp;
            }
        }
    }
    
    /* 闂傚倷绀侀幉锟犫€﹂崶鈺佸灊妞ゆ牗绮嶉弳婊堟煃瑜滈崜鐔煎蓟閿熺姴閱囨慨姗嗗厸婢规洖鈹戦悩顔肩仼闁绘牓鍨藉畷鐔碱敄鐠恒劌绗￠梻浣筋嚙缁绘劗鎹㈤幇顔藉床闁圭増婢橀悞鍨亜閹烘垵鈧摜鏁崼鏇熺厵妞ゆ梻鐡斿▓婊呪偓瑙勬穿缂嶄礁顕ｉ崐鐕佹▌闂佺懓澹婇崜鐔煎蓟?*/
    for (dat_index = TOUCH_READ_DISCARD; dat_index < (TOUCH_READ_TIMES - TOUCH_READ_DISCARD); dat_index++)
    {
        dat_sum += dat[dat_index];
    }
    
    return (dat_sum / (TOUCH_READ_TIMES - (TOUCH_READ_DISCARD << 1)));
}

/**
 * @brief       闂傚倷绀侀崥瀣磿閹惰棄搴婇柤鑹扮堪娴滃綊鏌涢妷銏℃珕闁哥姴妫濋弻銊╁籍閸ヮ灝鎾绘煕閵堝拋鍎旈柡宀嬬秮婵℃悂濡烽妷褌绨?Y闂備礁鎼ˇ閬嶅磻閻旂鈧倿鎮·澶愭⒒娴ｄ警鐒鹃柨鏇畵婵″鈧綆鍓氶～鏇熺箾閸℃ê濮夐柍缁樻閺屾洟宕煎┑鍥风礊闂佺鐟崶銊㈡嫼闂佸湱绮敮妤佸緞閸曨垱鐓熸俊銈傚亾鐎规洦鍓熼、娆掔疀濞戣鲸鏅╅柣蹇撶箲閻楁洘绂嶆ィ鍐╃厽闁规崘娅曢幑锝嗘叏閿濆懐澧曢棁澶嬬節婵犲倸鏆ｉ柛娆忓娣囧﹪骞撻幒鎾虫灎閻庤娲橀崝娆撳箠濠靛绠ユい鏃傝檸閸炲綊姊绘担瑙勫仩闁稿孩濞婂畷鎴濃槈濮楀棛鍔烽柟鍏肩暘閸斿瞼澹曢崷顓犳／妞ゆ挶鍩勫Λ鎴犵磼閳?
 * @param       cmd: TOUCH_CMD_X 闂?TOUCH_CMD_Y
 * @retval      闂傚倷绀侀崥瀣磿閹惰棄搴婇柤鑹扮堪娴滃綊鏌涢妷顔煎缂佲偓閸℃稒鐓欐繛鍫濈仢閺嬬喖鏌ｉ敐澶夋喚闁诡喖鍢查埢搴♀枎閹炬惌鈧秹姊虹涵鍛彧缂佸瞼绋楥闂? * @note        [Fix2] 濠电姷鏁搁崕鎴犵礊閳ь剚銇勯弴鍡楀閸欏繘鏌ｉ幇顒佹儓閻庢艾顦甸弻宥堫檨闁告挻鑹鹃銉╁礋椤曞懏鈻岄梻渚€娼荤徊浠嬪箺濠婂牆绠柣妯款嚙閸楁娊鏌ｉ弴姘鳖槮濞寸姴缍婂鐑樻姜閻楀牜妯傞梺鍛婎殔閸熷潡鈥栨繝鍋椽顢旈崟顓фТ闂佽崵濮崇粈浣革耿鏉堚晝鐭嗗鑸靛姈閳锋帡鏌涢幇鈺佸缂佺嫏鍕╀簻闁圭儤鎸鹃妴鎺撶箾閹寸姵鏆い銏＄墵楠炴劖鎯旈闂寸紦闂佽瀛╅鏍窗閺嶎厼纾归柟闂寸劍閸嬪鏌涢幘鑼额唹闁稿鎸婚幏鍛村礃椤垶顥嶉梻浣规偠娴煎洭宕惰閻嫰姊虹紒姗嗙劷缂侇噮鍨堕、鎾斥枎閹存柨浜鹃柣銏㈩焾娴滄澘霉濠婂懎浠遍柣?
 */
static uint16_t touch_get_adc2(uint8_t cmd)
{
    uint16_t dat1;
    uint16_t dat2;
    uint16_t delta;
    uint16_t retry = 0;  /* [Fix2] 闂傚倸鍊烽悞锕併亹閸愵亞鐭撻柣鎴ｅГ閸庡﹥銇勯弽銊х煁濠殿垰銈搁弻宥夊煛娴ｅ憡娈插┑鐐茬墕閻栧ジ寮?*/
    
    do {
        dat1 = touch_get_adc(cmd);
        dat2 = touch_get_adc(cmd);
        if (dat1 > dat2)
        {
            delta = dat1 - dat2;
        }
        else
        {
            delta = dat2 - dat1;
        }
        
        /* [Fix2] 闂備胶鍎甸崜婵堟暜閹烘绠犻柟鎹愬煐瀹曟煡鏌涢幇闈涙灈閻庢艾顦甸弻宥堫檨闁告挻鑹鹃銉╁礋椤曞懏鈻岄梻渚€娼荤徊浠嬪箺濠婂牆绠柣妯款嚙閸楁娊鏌ｉ弴姘鳖槮濞寸姴缍婂鐑樻姜閹殿噮妲柟鐓庣摠缁骸危閹邦兘鏀介柛銉ｅ妺濮橈箓姊洪崫鍕偓鍦偓绗涘懐鐭嗛柍褜鍓熼幃宄邦煥閸涱収鏆梺鍦焾閹诧繝寮查崼鏇熷亹闁惧浚鍋傚锕傛⒑閻熸壆鎽犵紒顔肩У娣囧﹨顦归柡灞剧⊕缁绘繈宕熼埞鎯т壕閻庯綆鍓氶～鏇熺節闂堟侗鍎愰柣鎺戙偢閺屾盯鈥﹂幋婵囩亪闁汇埄鍨靛▍鏇㈠箞閵娿儮鏀藉┑鐘查濞堫參姊虹拠鈥崇仩闁挎洦浜獮?*/
        if (++retry > TOUCH_ADC2_MAX_RETRY)
        {
            break;
        }
    } while (delta > TOUCH_READ_RANGE);
    
    return ((dat1 + dat2) >> 1);
}

/**
 * @brief       缂傚倸鍊搁崐鐑芥倿閿曞倵鈧箓宕堕鈧弸渚€鏌涢幘妤€瀚弸鍌炴⒑閹稿孩顥嗗┑顔哄€濆畷鐢碘偓锝庡枟閻撴稓鈧箍鍎扮拋鏌ュ磻閹捐绀冮柛娆忣槺瑜版煡姊洪懡銈呮瀾闁荤喖浜跺浠嬪礋椤栨氨鐤囧┑鐘诧工閻楀﹪宕? * @param       x    : 闂佽楠搁悘姘熆濮椻偓楠炲﹪骞囬弶鎸庣€俊銈忕到閸燁偊宕橀埀顒勬⒑閻愯棄鍔氶柛鐔风仢閳诲秴鈻庨幘鏉戜画濠电偛妫欓悷鈺侇瀶椤曗偓閺岀喖鎼归銈囩厜闂佽鍠楅悷鈺佺暦閻戠瓔鏁囬柣鏃傚劋濞堢珎闂傚倷鑳堕～瀣礋椤愩埄娼旈梻?
 * @param       y    : 闂佽楠搁悘姘熆濮椻偓楠炲﹪骞囬弶鎸庣€俊銈忕到閸燁偊宕橀埀顒勬⒑閻愯棄鍔氶柛鐔风仢閳诲秴鈻庨幘鏉戜画濠电偛妫欓悷鈺侇瀶椤曗偓閺岀喖鎼归銈囩厜闂佽鍠楅悷鈺佺暦閻戠瓔鏁囬柣鏃傚劋濞堢珒闂傚倷鑳堕～瀣礋椤愩埄娼旈梻?
 * @param       color: 闂佽楠搁悘姘熆濮椻偓楠炲﹪骞囬弶鎸庣€俊銈忕到閸燁偊宕橀埀顒勬⒑閻愯棄鍔氶柛鐔风仢閳诲秴鈻庨幘鏉戜画濠电偛妫欓悷鈺侇瀶椤曗偓閺岀喖鎼归銈囩厜闂佽鍠楅悷鈺佺暦閻戠瓔鏁囬柣鏃傚劋濞堢鈹戦埥鍡椾簽濠⒀勵殘閸掓帟绠涢弮鍌滃骄?
 */
static void touch_draw_touch_point(uint16_t x, uint16_t y, uint16_t color)
{
    LCD_DrawLine(x - 12, y, x + 13, y, color);
    LCD_DrawLine(x, y - 12, x, y + 13, color);
    LCD_DrawPoint(x + 1, y + 1, color);
    LCD_DrawPoint(x - 1, y + 1, color);
    LCD_DrawPoint(x + 1, y - 1, color);
    LCD_DrawPoint(x - 1, y - 1, color);
    LCD_DrawCircle(x, y, 6, color);
}

/**
 * @brief       闂備浇宕甸崰鎰版偡闁秵鈷旈柛鏇ㄥ灠缁犵姵绻濇繝鍌滃闁告劏鍋撻梻浣哄仺閸庨亶宕崹顔规瀺?
 * @note        [Fix1] 婵犵數鍎戠徊钘壝归崒鐐茬獥闁哄稁鍘旈崶顒€钃熼柕澶堝劚閸炪劑姊洪悙钘夊姎闁哥喎鐏濋埢宥呪枎韫囷絿鍞甸梺璇″灡婢瑰棛鑺遍崸妤佸仭婵炲棙鐟ч悾鐢告煛鐏炲€熷妞ゆ挸銈稿畷濂稿閳辨帪绠撳娲传閸曨偅娈查梺绋块閵堟悂骞冨鍫熸櫆闁兼亽鍎板锕傛⒑閹肩偛鍔楅柡鍛櫊瀹曟洟骞嬪┑鍐╊潔闂佸湱铏庨崹鐗堢閻愵剛绠鹃柟瀵稿仦閹嫬霉濠婂懎浠辩€规洘鍨块弫鎰緞鐎ｎ亜澹勯梻浣告啞鐢晠骞楃粔?
 *              [Fix3] 婵犵數鍋犻幓顏嗙礊閳ь剚绻涙径瀣鐎?osDelay 闂傚倷绀侀幖顐⒚洪妶澶娢︽慨妞诲亾鐎规洦鍓熼、妤呭礃椤忓棙婢戞繝鐢靛仜濡瑩宕硅ぐ鎺戠闁兼祴鏂侀崑鎾诲垂椤愶絿鍑￠梺褰掓敱閸ㄧ懓危閹邦兘鏀介悗锝庡亞閸斿爼姊洪崜鑼帥闁哥姵鐗滈弫顕€骞庨懞銉㈡嫽闂佸憡鍔﹂崣搴ㄥ吹濞嗘劑浜?RTOS
 *              [Fix4] 缂傚倸鍊搁崐鐑芥嚄閸洖绐楃€广儱娲ㄩ崡姘舵煛婢跺娈鹃柣鏃傗拡閺佸倿鏌涢弴銊ュ箺缂?缂傚倸鍊烽悞锕€顫忚ぐ鎺撳亱闁哄洢鍨洪崕鎾绘煕閳╁啰鈽夐柛鎰ㄥ亾闂備胶鍋ㄩ崕閬嶅疮閸喒鏋嶆繛鍡樻尰閻撴洟鐓崶椋庡埌濠⒀屽枤缁辨帡骞夌€ｎ剛鐦堥梺杞扮缁夋挳鎮惧┑瀣劦妞ゆ巻鍋撻悡銈夋煕瑜庨〃鍛不閹惰姤鐓忓璺虹墕閳ь剚娲熼崺銏ｇ疀濞戞瑧鍘藉銈庡亽閸撴瑦淇婃總鍛婄厽闁靛牆鎳庨顓熸叏婵犱胶鐭欑€规洜濞€閳ユ梹绻呴崒鐐寸厽闊洦鎸剧粻浼存煙绾板崬浜版い?
 *              [Fix8] 婵犵數鍋犻幓顏嗙礊閳ь剚绻涙径瀣鐎?float 闂傚倷绀侀幖顐⒚洪妶澶娢︽慨妞诲亾鐎?double闂傚倷鐒︾€笛呯矙閹达附鍎楅柛宀€鍋涢悞鍨亜閹哄棗浜鹃梺绋匡工濞尖€崇暦?STM32F4 闂傚倷绀侀幉锟犮€冮崱妞曞搫顭ㄩ崨鏉啃￠梺鍦焾鐎涒晠宕?FPU
 *              5婵犵數鍋為崹鍫曞箹閳哄倻顩叉繝闈涱儐閸嬪骞栧ǎ顒€濡肩紒鈧崟顖涚厱闁斥晛鍟伴幊鍐归悪鈧崹鍫曞蓟閵娿儮妲堟繛鍡樺笒椤ユ繃绻涢幋鐐寸叆缂佺粯鍨圭划娆愬緞鐎ｎ剛顔曢柟鐓庣摠閹稿憡绂嶉幆顬″綊鏁愰崶鍓佸姼缂備讲鍋撳ù锝囩《濡插牓鏌熼悙顒€澧柣鎺楃畺閺屽秷顧侀柛蹇旂〒缁牊鎷呴崷顓涙灆闂婎偄娲﹀濠氬煝閺冨牊鍊甸柨婵嗗暙婵＄兘鏌涚€ｎ偅灏伴柟宄版噽缁瑧鎹勯…鎴炐滄繝鐢靛仦閸ㄥ爼骞愰幖渚囨晞闁告劦鍠栭悞鍨亜閹寸偛顕滄い锔惧厴閺屻倝宕归銏紘缂? */
static void touch_calibration(void)
{
    struct
    {
        uint16_t x;
        uint16_t y;
    } point[5];
    uint8_t point_index;
    int16_t d1, d2, d3, d4;
    float x_fac, y_fac;
    uint16_t lcd_width = LCD_GetWidth();
    uint16_t lcd_height = LCD_GetHeight();
    const uint16_t cal_margin = TOUCH_CALIBRATION_MARGIN;
    uint16_t cal_x[5] = {
        cal_margin,
        (uint16_t)(lcd_width - cal_margin),
        cal_margin,
        (uint16_t)(lcd_width - cal_margin),
        (uint16_t)(lcd_width >> 1)
    };
    uint16_t cal_y[5] = {
        cal_margin,
        cal_margin,
        (uint16_t)(lcd_height - cal_margin),
        (uint16_t)(lcd_height - cal_margin),
        (uint16_t)(lcd_height >> 1)
    };

    while (1)
    {
        LCD_SetDirection(LCD_DIRECTION);
        LCD_Clear(COLOR_WHITE);

        for (point_index = 0; point_index < 5; point_index++)
        {
            if (point_index > 0U)
            {
                touch_draw_touch_point(cal_x[point_index - 1U], cal_y[point_index - 1U], COLOR_WHITE);
            }

            touch_draw_touch_point(cal_x[point_index], cal_y[point_index], COLOR_RED);

            while (TOUCH_READ_PEN() != 0)
            {
                osDelay(10);
            }

            osDelay(30);

            point[point_index].x = touch_get_adc2(TOUCH_CMD_X);
            point[point_index].y = touch_get_adc2(TOUCH_CMD_Y);

#if TOUCH_SWAP_XY
            {
                uint16_t temp = point[point_index].x;
                point[point_index].x = point[point_index].y;
                point[point_index].y = temp;
            }
#endif

            while (TOUCH_READ_PEN() == 0)
            {
                osDelay(10);
            }

            osDelay(30);
        }

        touch_draw_touch_point(cal_x[4], cal_y[4], COLOR_WHITE);

        d1 = (int16_t)point[1].x - (int16_t)point[0].x;
        d3 = (int16_t)point[3].x - (int16_t)point[2].x;
        d2 = (int16_t)point[3].y - (int16_t)point[1].y;
        d4 = (int16_t)point[2].y - (int16_t)point[0].y;

        if (d1 == 0 || d2 == 0 || d3 == 0 || d4 == 0)
        {
            osDelay(150);
            continue;
        }

        x_fac = (float)d1 / (float)d3;
        y_fac = (float)d2 / (float)d4;

        if (x_fac < 0.0f) x_fac = -x_fac;
        if (y_fac < 0.0f) y_fac = -y_fac;

        if (x_fac < 0.95f || x_fac > 1.05f || y_fac < 0.95f || y_fac > 1.05f ||
            abs(d1) > 4095 || abs(d2) > 4095 || abs(d3) > 4095 || abs(d4) > 4095)
        {
            osDelay(150);
            continue;
        }

        g_touch_sta.fac.x = ((float)d1 + (float)d3) / (2.0f * (float)(lcd_width - (cal_margin << 1)));
        g_touch_sta.fac.y = ((float)d2 + (float)d4) / (2.0f * (float)(lcd_height - (cal_margin << 1)));
        g_touch_sta.center.x = point[4].x;
        g_touch_sta.center.y = point[4].y;

        if ((g_touch_sta.fac.x > -0.001f && g_touch_sta.fac.x < 0.001f) ||
            (g_touch_sta.fac.y > -0.001f && g_touch_sta.fac.y < 0.001f))
        {
            osDelay(150);
            continue;
        }

        LCD_Clear(COLOR_WHITE);
        LCD_DisplayOn();
        LCD_BacklightOn();
        LCD_SetDirection(LCD_DIRECTION);
        return;
    }
}

/**
 * @brief       濠电姷顣藉Σ鍛村磻閳ь剟鏌涚€ｎ偅宕岄柡宀嬬磿娴狅妇鎷犻幓鎺懶撻柣鐐寸缁诲牓寮婚悢鍏煎癄濠㈣泛顑呴弳鍫濃攽閳ュ啿绾ч柟顔煎€搁悾鐑藉Ψ閳哄倹娅嗛梺鑺ッˇ鍗炍熼崱妯肩? * @retval      1: 闂傚倷绀佸﹢閬嶁€﹂崼銉嬪洭顢欓崜褏鐣? 0: 闂傚倷绀侀幖顐︽偋濠婂嫮顩叉繝闈涚墐閸嬫捇宕归銈囩厑缂? */
uint8_t touch_is_pressed(void)
{
    return (TOUCH_READ_PEN() == 0) ? 1 : 0;
}

/**
 * @brief       闂備浇宕甸崰鎰版偡闁秵鈷旈柛鏇ㄥ灠缁犵姵绻濇繝鍌滃缂佲偓閸℃绡€闂傚牊绋掗敍宥嗙箾閸忕⒈娈滈柡? * @note        濠电姵顔栭崰妤冩崲閹邦喖绶ら柛褎顨呯壕濠氭煥濠靛棭妲搁柛灞诲姂閺岋綁寮崼顐ｎ棖闂佺妫勭粔褰掑箖濡ゅ啯鍠嗛柛鏇炴閸ㄥ灝顕ｆ繝姘亜闁告縿鍎哄ù鍕⒑閹稿海绠撴繛灞傚姂瀹曞綊骞庨懞銉у弳濠电偞鍨堕悷褔銆傞弻銉︾厽闁挎柨鍘滈崑鎾诲棘閵夛妇鈧姊洪悙钘夊姤閻忓繑鐟╅幃鐑藉箛閻楀牏鍘介梺闈涚箞閸ㄦ椽寮抽鍕厱闊洦鏋忛幋鐘亾閻㈤潧甯舵い顐ｇ箓閻ｇ兘宕堕…鎴濇櫃闂傚倷鐒﹀鍨焽閸ф绀夌€广儱顦伴崑瀣箹濞ｎ剙濡肩紒鈧? */
void touch_init(void)
{
    /* 闂傚倷绀侀幉锛勬暜濡ゅ啯宕查柛宀€鍎戠紞鏍煙閻楀牊绶茬紒鈧畝鍕厸鐎广儱楠搁崝顕€鏌￠崶銉ョ仼缂佺姵妞介弻娑樷攽閸曨偄濮夐梺閫炲苯澧柣妤侇殘缁?*/
    touch_hw_init();
    touch_spi_init();
    
    /* 闂備浇宕甸崰鎰版偡闁秵鈷旈柛鏇ㄥ灠缁犵姵绻濇繝鍌滃闁告劏鍋撻梻浣哄仺閸庨亶宕崹顔规瀺?- 濠电姵顔栭崰妤冩崲閹邦喖绶ら柛褎顨呯壕濠氭煥濠靛棭妲搁柛灞诲姂閺岋綁寮崼顐ｎ棖闂佺妫勭粔褰掑箖濡ゅ啯鍠嗛柛鏇炴閸ㄥ灝顕ｆ繝姘亜闁告縿鍎哄ù鍕⒑閹稿海绠撴繛灞傚姂瀹曞綊鎼归锝呭伎闂佹寧绻傞幊鎰不閸欏浜滈柟閭﹀墮閹垹绱掔€ｎ亶妲圭紒缁樼箞瀹曠喖顢楁担鍦闂傚倸鍊风欢锟犲窗濞戞瑦鍙忛柕鍫濐槹閸嬪骞栧ǎ顒€濡肩紒鈧?*/
    touch_calibration();
}

/**
 * @brief       闂備浇宕甸崰鎰版偡闁秵鈷旈柛鏇ㄥ灠缁犵姵绻濇繝鍌滃缂佺姰鍎查妵鍕箛閸撲礁鍩屾繛?
 * @param       x: 闂傚倷娴囬～澶愵敊閺嶎厼纾婚柛娑卞弾閸ゆ洟鏌涜椤ㄥ懐绮堥崱娑欑厵鐎瑰嫭澹嗗锟犳煛閸ャ儱鐏╃紒鐘虫そ閺屾稑鈹戦崟顐㈠闂佷紮绲垮ú顖炴⒒娴ｄ警娼掗柛鏇ㄥ亜椤秹姊?
 * @param       y: 闂傚倷娴囬～澶愵敊閺嶎厼纾婚柛娑卞弾閸ゆ洟鏌涜椤ㄥ懐绮堥崱娑欑厵鐎瑰嫭澹嗗锟犳煛閸ャ儱鐏╃紒鐘虫そ閺屾稑鈹戦崟顐㈠闂佷紮绲垮ú鍫曟⒒娴ｄ警娼掗柛鏇ㄥ亜椤秹姊?
 * @retval      TOUCH_EOK: 闂傚倷娴囬～澶愵敊閺嶎厼纾婚柛娑卞弾閸ゆ洟鏌涜椤ㄥ懐绮堥崱娑欑厽婵°倐鍋撻柣妤€锕︾划濠氬箳濡や胶鍘告繛杈剧到閹芥粍鏅堕姀锛勭闁割偆鍠庨悘鈺呮煙閸欏灏﹂柡浣稿€块幊鐐哄Ψ閵夛妇鏉?
 * @retval      TOUCH_ERROR: 闂備浇宕甸崰鎰版偡闁秵鈷旈柛鏇ㄥ灠缁犵姵绻濇繝鍌滃闂佽￥鍊濋弻鐔兼焽閿曗偓楠炴﹢鏌涢妸銉ｅ仮闁哄矉绻濆畷姗€顢旈崟鎴秮閺?
 * @retval      TOUCH_EMPTY: 闂傚倷绀侀幖顐︽偋濠婂嫮顩叉繝闈涙川閻濆爼鏌涘畝鈧崑娑氱不閻㈠摜鍙撻柛銉ｅ妽鐏忣參鏌ｉ幘鍐测偓鍧楀蓟閿熺姴閱囨繝鍨姈鏁堥梻浣告惈閹冲矂宕曢悽绋跨畾闁告劦鍠栫粈瀣亜閺傜偛瀚粈瀣殽? * @note        [Fix4] 缂傚倸鍊风粈渚€藝椤栫偐鈧箑鐣￠幍铏€洪柟鍏肩暘閸斿瞼娑甸埀顒勬⒑閸濆嫭宸濆┑顔芥尦椤㈡棃顢旈崼鐔哄幗闂侀潧顭堥崕閬嶎敂椤忓牊鐓ｉ柛鈩冪⊕閸婂爼鏌ｉ幇顖氱毢缂佺姷澧楃换娑㈠礂绾拌鲸鐤侀悗娈垮枙缁瑥鐣烽幆閭︽Щ缂傚倸绉崇欢姘跺蓟閻旂儤瀚氶柍鈺佸暟缁愭瑧绱撴担浠嬪摵缂佽鐗嗛锝嗙鐎ｎ亞浼嬮梺鍛婂姇瀵爼鎮甸鐐村€甸柣鐔哄閸熺偟鎲搁弶鍨殭闁挎洏鍨介、鏃堝幢韫囨挷澹曢梺闈╁瘜閸樺吋绂掓潏銊ょ箚闁告挆鍕虎CD闂傚倷鑳堕～瀣礋椤愩埄娼旈梻浣虹帛閻楊厾寰婇崸妤€绀岄柡宥庡亝婵偐鈧娲栧ú銊╁箟椤曗偓閺岋絾鎯旈敐鍡楁畬闂佸搫鐗滈崜鐔风暦?
 */
uint8_t touch_scan(uint16_t *x, uint16_t *y)
{
    uint16_t x_adc;
    uint16_t y_adc;
    int16_t x_raw;
    int16_t y_raw;
    int16_t lcd_width = (int16_t)LCD_GetWidth();
    int16_t lcd_height = (int16_t)LCD_GetHeight();
    const int16_t out_of_range_margin = (int16_t)TOUCH_CALIBRATION_MARGIN;

    if (TOUCH_READ_PEN() == 0)
    {
        x_adc = touch_get_adc2(TOUCH_CMD_X);
        y_adc = touch_get_adc2(TOUCH_CMD_Y);

#if TOUCH_SWAP_XY
        {
            uint16_t temp = x_adc;
            x_adc = y_adc;
            y_adc = temp;
        }
#endif

        if ((g_touch_sta.fac.x > -0.001f && g_touch_sta.fac.x < 0.001f) ||
            (g_touch_sta.fac.y > -0.001f && g_touch_sta.fac.y < 0.001f))
        {
            return TOUCH_ERROR;
        }

        x_raw = (int16_t)((((float)((int32_t)x_adc - (int32_t)g_touch_sta.center.x)) / g_touch_sta.fac.x) + ((float)lcd_width / 2.0f));
        y_raw = (int16_t)((((float)((int32_t)y_adc - (int32_t)g_touch_sta.center.y)) / g_touch_sta.fac.y) + ((float)lcd_height / 2.0f));

        if ((x_raw < -out_of_range_margin) || (x_raw >= (lcd_width + out_of_range_margin)) ||
            (y_raw < -out_of_range_margin) || (y_raw >= (lcd_height + out_of_range_margin)))
        {
            return TOUCH_ERROR;
        }

        if (x_raw < 0) x_raw = 0;
        else if (x_raw >= lcd_width) x_raw = lcd_width - 1;

        if (y_raw < 0) y_raw = 0;
        else if (y_raw >= lcd_height) y_raw = lcd_height - 1;

        *x = (uint16_t)x_raw;
        *y = (uint16_t)y_raw;
        return TOUCH_EOK;
    }

    return TOUCH_EMPTY;
}

/**
 * @brief       闂備浇宕甸崰鎰版偡闁秵鈷旈柛鏇ㄥ灠缁犵姵绻濇繝鍌涘櫤闁稿繑绮嶉妵鍕箣閻愬樊鍤嬪銈嗗姌婵倗鎹㈠☉銏犵骇闁规惌鍘奸崜鐢告⒑?
 * @param       x_adc: X闂傚倷鑳堕～瀣礋椤愩埄娼旈梻浣虹帛閻楊剟寮搁崓涓嗛梻鍌欒兌椤牓鏁冮妸锔剧彾闁割偁鍨洪鑺ャ亜閹惧崬鐏╃紒鈧? * @param       y_adc: Y闂傚倷鑳堕～瀣礋椤愩埄娼旈梻浣虹帛閻楊剟寮搁崓涓嗛梻鍌欒兌椤牓鏁冮妸锔剧彾闁割偁鍨洪鑺ャ亜閹惧崬鐏╃紒鈧? * @retval      PEN闂傚倷鑳堕崢褔鎮洪妸銉僵闁靛闄勯崣蹇涙煙闂傚顦︾紒?(0=闂傚倷绀佸﹢閬嶁€﹂崼銉嬪洭顢欓崜褏鐣? 1=闂傚倷绀侀幖顐︽偋濠婂嫮顩叉繝闈涚墐閸嬫捇宕归銈囩厑缂?
 */
uint8_t touch_test(uint16_t *x_adc, uint16_t *y_adc)
{
    uint8_t pen_state;
    
    /* PEN婵犵數鍋為崹鍫曞箰閹间焦鏅濋柕澶涚畱閸ㄦ棃鏌涘┑鍡楃弸闁绘梻鍘ч悘宕団偓瑙勬礀濞层劑銆侀崨瀛樷拺闁圭娴烽埊鏇犵棯閺夎法肖闁逞屽墮閻忔氨鍒掑鍥╃煓濠㈣泛澶囬崑鎾绘晲鎼粹€茬凹濡炪倧瀵屽﹢鎵橬=0 */
    pen_state = TOUCH_READ_PEN();
    
    if (x_adc != NULL) {
        *x_adc = touch_spi_read(TOUCH_CMD_X);
    }
    if (y_adc != NULL) {
        *y_adc = touch_spi_read(TOUCH_CMD_Y);
    }
    
    return pen_state;
}
