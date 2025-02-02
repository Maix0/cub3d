SRC_FILES =                                                                   \
aq/lc_alloc/functions1                                                        \
aq/lc_alloc/functions2                                                        \
aq/me_alloc/find_block                                                        \
aq/me_alloc/functions1                                                        \
aq/me_alloc/functions2                                                        \
aq/me_alloc/internals                                                         \
aq/me_alloc/merge_blocks                                                      \
aq/me_alloc/pages                                                             \
aq/me_alloc/realloc                                                           \
aq/vg/dummy_block                                                             \
aq/vg/dummy_mem_status                                                        \
aq/vg/dummy_mempool                                                           \
aq/vg/dummy_mempool_bis                                                       \
aq/vg/valgrind_block                                                          \
aq/vg/valgrind_mem_status                                                     \
aq/vg/valgrind_mempool                                                        \
aq/vg/valgrind_mempool_bis                                                    \
blx/blx                                                                       \
blx/blx_create_fontsheet                                                      \
blx/blx_handlers                                                              \
blx/blx_keycode                                                               \
blx/colors                                                                    \
blx/draw/draw                                                                 \
blx/draw/draw_circle                                                          \
blx/draw/draw_line                                                            \
blx/draw/draw_rect                                                            \
blx/draw/draw_sprite                                                          \
blx/draw/draw_string                                                          \
blx/inputs                                                                    \
blx/logic                                                                     \
blx/mouse                                                                     \
blx/sprite/draw_image                                                         \
blx/sprite/draw_pixel_onto                                                    \
blx/sprite/draw_string                                                        \
blx/sprite/free_image                                                         \
blx/sprite/get_pixel                                                          \
blx/sprite/new_image                                                          \
blx/sprite/sprite_draw_onto_sprite                                            \
char/isalnum                                                                  \
char/isalpha                                                                  \
char/isascii                                                                  \
char/isdigit                                                                  \
char/islower                                                                  \
char/isprint                                                                  \
char/isspace                                                                  \
char/isupper                                                                  \
char/tolower                                                                  \
char/toupper                                                                  \
convert/numbers_to_str                                                        \
fs/directory                                                                  \
fs/fd                                                                         \
fs/file                                                                       \
fs/file_dup                                                                   \
fs/fs_internal                                                                \
fs/getline                                                                    \
fs/getters                                                                    \
fs/putfd                                                                      \
gnl/get_next_line                                                             \
gnl/gnl2                                                                      \
hash/hash_signed                                                              \
hash/hash_str                                                                 \
hash/hash_unsigned                                                            \
hash/hasher                                                                   \
hash/sip/sip13                                                                \
hash/sip/sip_utils                                                            \
hash/sip/sip_utils2                                                           \
img/qoi/qoi_decode                                                            \
img/qoi/qoi_encode                                                            \
img/qoi/qoi_fs                                                                \
img/qoi/qoi_utils                                                             \
mem/allocator                                                                 \
mem/mem_alloc                                                                 \
mem/mem_alloc_array                                                           \
mem/mem_compare                                                               \
mem/mem_copy                                                                  \
mem/mem_find                                                                  \
mem/mem_find_bytes                                                            \
mem/mem_move                                                                  \
mem/mem_realloc                                                               \
mem/mem_set                                                                   \
mem/mem_set_zero                                                              \
num/f64/f64_clamp                                                             \
num/u16/rotate                                                                \
num/u32/rotate                                                                \
num/u64/from_bytes                                                            \
num/u64/rotate                                                                \
num/u8/rotate                                                                 \
num/usize/rotate                                                              \
num/usize/round_up                                                            \
os/abort                                                                      \
os/exit                                                                       \
os/pipe                                                                       \
os/process                                                                    \
os/process_iterator_function                                                  \
os/process_redirection                                                        \
printf/callbacks                                                              \
printf/formatter/char                                                         \
printf/formatter/decimal                                                      \
printf/formatter/hex                                                          \
printf/formatter/oct                                                          \
printf/formatter/ptr                                                          \
printf/formatter/unsigned_decimal                                             \
printf/formatter/utils                                                        \
printf/formatter/utils2                                                       \
printf/formatter/utils3                                                       \
printf/formatter/utils_numbers                                                \
printf/matchers                                                               \
printf/printf                                                                 \
printf/printf_fd                                                              \
printf/printf_str                                                             \
printf/vprintf                                                                \
str/str_charset                                                               \
str/str_clone                                                                 \
str/str_compare                                                               \
str/str_find_chr                                                              \
str/str_find_rev_chr                                                          \
str/str_find_str                                                              \
str/str_iter                                                                  \
str/str_join                                                                  \
str/str_l_cat                                                                 \
str/str_l_copy                                                                \
str/str_len                                                                   \
str/str_map                                                                   \
str/str_n_compare                                                             \
str/str_n_find_str                                                            \
str/str_prefixes_suffixes                                                     \
str/str_split                                                                 \
str/str_substring                                                             \
str/str_trim                                                                  \
string/mod                                                                    \
string/string_insert                                                          \
string/string_remove                                                          \
string/string_reserve                                                         \

GEN_FILES =                                                                   \
convert/i16_to_str                                                            \
convert/i32_to_str                                                            \
convert/i64_to_str                                                            \
convert/i8_to_str                                                             \
convert/str_to_i16                                                            \
convert/str_to_i16_utils                                                      \
convert/str_to_i32                                                            \
convert/str_to_i32_utils                                                      \
convert/str_to_i64                                                            \
convert/str_to_i64_utils                                                      \
convert/str_to_i8                                                             \
convert/str_to_i8_utils                                                       \
convert/str_to_u16                                                            \
convert/str_to_u16_utils                                                      \
convert/str_to_u32                                                            \
convert/str_to_u32_utils                                                      \
convert/str_to_u64                                                            \
convert/str_to_u64_utils                                                      \
convert/str_to_u8                                                             \
convert/str_to_u8_utils                                                       \
convert/u16_to_str                                                            \
convert/u32_to_str                                                            \
convert/u64_to_str                                                            \
convert/u8_to_str                                                             \
vec/str/str                                                                   \
vec/str/str_functions2                                                        \
vec/str/str_functions3                                                        \
vec/str/str_sort                                                              \
vec/u8/u8                                                                     \
vec/u8/u8_functions2                                                          \
vec/u8/u8_functions3                                                          \

