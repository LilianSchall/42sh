project('42sh', 'c', 
  version : '0.1',
  default_options : [
    'warning_level=3',
    'c_std=c99',
    'c_args=-D_POSIX_C_SOURCE=200809L -pedantic -Werror -Wall -Wextra -Wvla -Isrc -g',
  ])

inc = include_directories('src')

subdir('src')


executable('42sh',
  sources : [src],
  include_directories : inc)
