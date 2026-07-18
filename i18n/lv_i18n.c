#include "./lv_i18n.h"
#include "../pages/ui_popup_language.h"

////////////////////////////////////////////////////////////////////////////////
// Define plural operands
// http://unicode.org/reports/tr35/tr35-numbers.html#Operands

// Integer version, simplified

#define UNUSED(x) (void)(x)

static inline uint32_t op_n(int32_t val) { return (uint32_t)(val < 0 ? -val : val); }
static inline uint32_t op_i(uint32_t val) { return val; }
// always zero, when decimal part not exists.
static inline uint32_t op_v(uint32_t val) { UNUSED(val); return 0;}
static inline uint32_t op_w(uint32_t val) { UNUSED(val); return 0; }
static inline uint32_t op_f(uint32_t val) { UNUSED(val); return 0; }
static inline uint32_t op_t(uint32_t val) { UNUSED(val); return 0; }

// 英语
static lv_i18n_phrase_t en_us_singulars[] = {
};


static uint8_t en_us_plural_fn(int32_t num)
{
    uint32_t n = op_n(num); UNUSED(n);
    uint32_t i = op_i(n); UNUSED(i);
    uint32_t v = op_v(n); UNUSED(v);

    if ((i == 1 && v == 0)) return LV_I18N_PLURAL_TYPE_ONE;
    return LV_I18N_PLURAL_TYPE_OTHER;
}

static lv_i18n_lang_t en_us_lang = {
    .locale_name = "en-US",
    .singulars = NULL,

    .locale_plural_fn = en_us_plural_fn
};

// 德语
static lv_i18n_phrase_t de_de_singulars[] = {
    {NULL, NULL}, // End mark
};


static uint8_t de_de_plural_fn(int32_t num)
{
    uint32_t n = op_n(num); UNUSED(n);
    uint32_t i = op_i(n); UNUSED(i);
    uint32_t v = op_v(n); UNUSED(v);

    if ((i == 1 && v == 0)) return LV_I18N_PLURAL_TYPE_ONE;
    return LV_I18N_PLURAL_TYPE_OTHER;
}

static lv_i18n_lang_t de_de_lang = {
    .locale_name = "de-DE",
    .singulars = NULL,

    .locale_plural_fn = de_de_plural_fn
};

// 希腊语
static lv_i18n_phrase_t el_gr_singulars[] = {

};


static uint8_t el_gr_plural_fn(int32_t num)
{
    uint32_t n = op_n(num); UNUSED(n);


    if ((n == 1)) return LV_I18N_PLURAL_TYPE_ONE;
    return LV_I18N_PLURAL_TYPE_OTHER;
}

static lv_i18n_lang_t el_gr_lang = {
    .locale_name = "el-GR",
    .singulars = NULL,

    .locale_plural_fn = el_gr_plural_fn
};

// 西班牙语
static lv_i18n_phrase_t es_es_singulars[] = {
    {NULL, NULL}, // End mark
};



static uint8_t es_es_plural_fn(int32_t num)
{
    uint32_t n = op_n(num); UNUSED(n);


    if ((n == 1)) return LV_I18N_PLURAL_TYPE_ONE;
    return LV_I18N_PLURAL_TYPE_OTHER;
}

static lv_i18n_lang_t es_es_lang = {
    .locale_name = "es-ES",
    .singulars = NULL,

    .locale_plural_fn = es_es_plural_fn
};

// 芬兰语
static lv_i18n_phrase_t fi_fi_singulars[] = {
    {NULL, NULL}, // End mark
};



static uint8_t fi_fi_plural_fn(int32_t num)
{
    uint32_t n = op_n(num); UNUSED(n);
    uint32_t i = op_i(n); UNUSED(i);
    uint32_t v = op_v(n); UNUSED(v);

    if ((i == 1 && v == 0)) return LV_I18N_PLURAL_TYPE_ONE;
    return LV_I18N_PLURAL_TYPE_OTHER;
}

static lv_i18n_lang_t fi_fi_lang = {
    .locale_name = "fi-FI",
    .singulars = NULL,

    .locale_plural_fn = fi_fi_plural_fn
};

// 法语
static lv_i18n_phrase_t fr_fr_singulars[] = {
    {NULL,NULL}
} ;



static uint8_t fr_fr_plural_fn(int32_t num)
{
    uint32_t n = op_n(num); UNUSED(n);
    uint32_t i = op_i(n); UNUSED(i);

    if ((((i == 0) || (i == 1)))) return LV_I18N_PLURAL_TYPE_ONE;
    return LV_I18N_PLURAL_TYPE_OTHER;
}

static lv_i18n_lang_t fr_fr_lang = {
    .locale_name = "fr-FR",
    .singulars = NULL,

    .locale_plural_fn = fr_fr_plural_fn
};

// 意大利语
static lv_i18n_phrase_t it_it_singulars[] = {
    {NULL,NULL}
} ;

static uint8_t it_it_plural_fn(int32_t num)
{
    uint32_t n = op_n(num); UNUSED(n);
    uint32_t i = op_i(n); UNUSED(i);
    uint32_t v = op_v(n); UNUSED(v);

    if ((i == 1 && v == 0)) return LV_I18N_PLURAL_TYPE_ONE;
    return LV_I18N_PLURAL_TYPE_OTHER;
}

static lv_i18n_lang_t it_it_lang = {
    .locale_name = "it-IT",
    .singulars = NULL,

    .locale_plural_fn = it_it_plural_fn
};


// 葡萄牙语
static lv_i18n_phrase_t pt_pt_singulars[] = {
    {NULL,NULL}
} ;

static uint8_t pt_pt_plural_fn(int32_t num)
{
    uint32_t n = op_n(num); UNUSED(n);
    uint32_t i = op_i(n); UNUSED(i);
    uint32_t v = op_v(n); UNUSED(v);

    if ((i == 1 && v == 0)) return LV_I18N_PLURAL_TYPE_ONE;
    return LV_I18N_PLURAL_TYPE_OTHER;
}

static lv_i18n_lang_t pt_pt_lang = {
    .locale_name = "pt-PT",
    .singulars = NULL,
    .locale_plural_fn = pt_pt_plural_fn
};

// 荷兰语
static lv_i18n_phrase_t nl_nl_singulars[] = {
    {NULL, NULL}, // End mark
};



static uint8_t nl_nl_plural_fn(int32_t num)
{
    uint32_t n = op_n(num); UNUSED(n);
    uint32_t i = op_i(n); UNUSED(i);
    uint32_t v = op_v(n); UNUSED(v);

    if ((i == 1 && v == 0)) return LV_I18N_PLURAL_TYPE_ONE;
    return LV_I18N_PLURAL_TYPE_OTHER;
}

static lv_i18n_lang_t nl_nl_lang = {
    .locale_name = "nl-NL",
    .singulars = NULL,

    .locale_plural_fn = nl_nl_plural_fn
};

// 俄语
static lv_i18n_phrase_t ru_ru_singulars[] = {
    {NULL,NULL}
} ;

static uint8_t ru_ru_plural_fn(int32_t num)
{
    uint32_t n = op_n(num); UNUSED(n);
    uint32_t v = op_v(n); UNUSED(v);
    uint32_t i = op_i(n); UNUSED(i);
    uint32_t i10 = i % 10;
    uint32_t i100 = i % 100;
    if ((v == 0 && i10 == 1 && i100 != 11)) return LV_I18N_PLURAL_TYPE_ONE;
    if ((v == 0 && (2 <= i10 && i10 <= 4) && (!(12 <= i100 && i100 <= 14)))) return LV_I18N_PLURAL_TYPE_FEW;
    if ((v == 0 && i10 == 0) || (v == 0 && (5 <= i10 && i10 <= 9)) || (v == 0 && (11 <= i100 && i100 <= 14))) return LV_I18N_PLURAL_TYPE_MANY;
    return LV_I18N_PLURAL_TYPE_OTHER;
}

static lv_i18n_lang_t ru_ru_lang = {
    .locale_name = "ru-RU",
    .singulars = NULL,

    .locale_plural_fn = ru_ru_plural_fn
};

lv_i18n_language_pack_t lv_i18n_language_pack[] = {
    &en_us_lang,
    &de_de_lang,
    &el_gr_lang,
    &es_es_lang,
    &fi_fi_lang,
    &fr_fr_lang,
    &it_it_lang,
    &pt_pt_lang,
    &nl_nl_lang,
    &ru_ru_lang,
    NULL // End mark
};

////////////////////////////////////////////////////////////////////////////////


// Internal state
static lv_i18n_language_pack_t * current_lang_pack;
static lv_i18n_lang_t * current_lang;


lv_i18n_lang_t * current_lang_point;
/**
 * Reset internal state. For testing.
 */
void __lv_i18n_reset(void)
{
    current_lang_pack = NULL;
    current_lang = NULL;
}

/**
 * Set the languages for internationalization
 * @param langs pointer to the array of languages. (Last element has to be `NULL`)
 */
int lv_i18n_init(lv_i18n_language_pack_t * langs)
{
    if(langs == NULL) return -1;
    if(langs[0] == NULL) return -1;

    current_lang_pack = langs;
    current_lang = langs[0];     /*Automatically select the first language*/
    parse_all_lang_jsons();

    return 0;
}

/**
 * Change the localization (language)
 * @param l_name name of the translation locale to use. E.g. "en-GB"
 */
int lv_i18n_set_locale(const char * l_name)
{
    if(current_lang_pack == NULL) return -1;

    uint16_t i;

    for(i = 0; current_lang_pack[i] != NULL; i++) {
        // Found -> finish
        if(strcmp(current_lang_pack[i]->locale_name, l_name) == 0) {
            current_lang = current_lang_pack[i];
            current_lang_point = current_lang;
            return 0;
        }
    }

    return -1;
}


static const char * __lv_i18n_get_text_core(lv_i18n_phrase_t * trans, const char * msg_id)
{
    uint16_t i;
    for(i = 0; trans[i].msg_id != NULL; i++) {
        if(strcmp(trans[i].msg_id, msg_id) == 0) {
            /*The msg_id has found. Check the translation*/
            if(trans[i].translation) return trans[i].translation;
        }
    }

    return NULL;
}


/**
 * Get the translation from a message ID
 * @param msg_id message ID
 * @return the translation of `msg_id` on the set local
 */
const char * lv_i18n_get_text(const char * msg_id)
{
    if(current_lang == NULL) return msg_id;

    const lv_i18n_lang_t * lang = current_lang;
    const void * txt;

    // Search in current locale
    if(lang->singulars != NULL) {
        txt = __lv_i18n_get_text_core(lang->singulars, msg_id);
        if (txt != NULL) return txt;
    }

    // Try to fallback
    if(lang == current_lang_pack[0]) return msg_id;
    lang = current_lang_pack[0];

    // Repeat search for default locale
    if(lang->singulars != NULL) {
        txt = __lv_i18n_get_text_core(lang->singulars, msg_id);
        if (txt != NULL) return txt;
    }

    return msg_id;
}

/**
 * Get the translation from a message ID and apply the language's plural rule to get correct form
 * @param msg_id message ID
 * @param num an integer to select the correct plural form
 * @return the translation of `msg_id` on the set local
 */
const char * lv_i18n_get_text_plural(const char * msg_id, int32_t num)
{
    if(current_lang == NULL) return msg_id;

    const lv_i18n_lang_t * lang = current_lang;
    const void * txt;
    lv_i18n_plural_type_t ptype;

    // Search in current locale
    if(lang->locale_plural_fn != NULL) {
        ptype = lang->locale_plural_fn(num);

        if(lang->plurals[ptype] != NULL) {
            txt = __lv_i18n_get_text_core(lang->plurals[ptype], msg_id);
            if (txt != NULL) return txt;
        }
    }

    // Try to fallback
    if(lang == current_lang_pack[0]) return msg_id;
    lang = current_lang_pack[0];

    // Repeat search for default locale
    if(lang->locale_plural_fn != NULL) {
        ptype = lang->locale_plural_fn(num);

        if(lang->plurals[ptype] != NULL) {
            txt = __lv_i18n_get_text_core(lang->plurals[ptype], msg_id);
            if (txt != NULL) return txt;
        }
    }

    return msg_id;
}

/**
 * Get the name of the currently used locale.
 * @return name of the currently used locale. E.g. "en-GB"
 */
const char * lv_i18n_get_current_locale(void)
{
    if(!current_lang) return NULL;
    return current_lang->locale_name;
}
