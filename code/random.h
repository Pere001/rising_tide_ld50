
//
// Basic random functions
//

#ifndef RANDOM_H
#define RANDOM_H

// Size: 6000
static u8 globalRandomTable[] = {
0xaa, 0x25, 0x45, 0x87, 0x30, 0x87, 0x7f, 0x5c, 0xe9, 0xc0, 0x3d, 0x86, 0x7e, 0xfb, 0xf6, 0x52,
0x6a, 0x1c, 0x74, 0x7d, 0xa0, 0x57, 0x93, 0x75, 0xae, 0x4b, 0xee, 0xca, 0x04, 0x2d, 0xb6, 0xfa,
0xe6, 0xbf, 0x02, 0x03, 0x32, 0xbf, 0xb8, 0x5e, 0x1f, 0xb0, 0x3c, 0x3d, 0xbe, 0xd7, 0xd4, 0xab,
0xd2, 0xeb, 0x74, 0x5e, 0xa3, 0xc6, 0xb3, 0x59, 0xa5, 0xb6, 0x4f, 0x1a, 0x44, 0xc9, 0x05, 0x96,
0xa0, 0xf6, 0x16, 0x40, 0x78, 0xf0, 0x31, 0xe6, 0xd3, 0xfb, 0x14, 0x40, 0x1c, 0x7c, 0x8e, 0xc8,
0x97, 0x16, 0x9e, 0x7e, 0x71, 0x66, 0xfb, 0x10, 0x1d, 0x32, 0xb2, 0xdf, 0xd2, 0xe6, 0x5f, 0xf5,
0xff, 0x5b, 0xbf, 0x9d, 0x6e, 0x52, 0xc9, 0xfe, 0x4e, 0xe6, 0xdd, 0x01, 0x95, 0x7a, 0x7a, 0x34,
0xe7, 0x06, 0x73, 0xcc, 0x04, 0x42, 0xa3, 0x8a, 0x37, 0xb1, 0xb0, 0x38, 0xf4, 0x21, 0x5a, 0x61,
0xd8, 0x47, 0xdb, 0x6b, 0xa8, 0x9a, 0xdc, 0x47, 0x5c, 0xe7, 0x4a, 0x56, 0x77, 0xef, 0x97, 0x31,
0xbc, 0xeb, 0x43, 0x90, 0xef, 0x9d, 0xe0, 0x7d, 0xf3, 0x1a, 0x16, 0x37, 0xae, 0xe8, 0xd9, 0x69,
0x6f, 0x44, 0x3f, 0x49, 0x2f, 0x16, 0xab, 0x8f, 0xea, 0xe7, 0x1b, 0x8a, 0x7a, 0xaf, 0x2e, 0x0e,
0x4d, 0x2c, 0x75, 0xef, 0xbc, 0x89, 0x64, 0x42, 0x85, 0x0f, 0x37, 0xa6, 0x3e, 0x8e, 0x2c, 0xf6,
0x8d, 0x99, 0xda, 0x4b, 0x58, 0x64, 0xc0, 0xd0, 0x6a, 0xe9, 0x34, 0x80, 0x31, 0xcc, 0xca, 0x60,
0x48, 0x69, 0x22, 0xbb, 0x31, 0x1e, 0x4f, 0xae, 0xf1, 0xae, 0x73, 0x09, 0x6b, 0xe8, 0x5f, 0x9c,
0x7c, 0xda, 0xb4, 0x32, 0x0c, 0x9c, 0xc2, 0x36, 0xc6, 0xd7, 0x0f, 0x63, 0x3b, 0x85, 0x7a, 0x44,
0x35, 0x99, 0xee, 0x52, 0xc3, 0xc1, 0x9e, 0x57, 0xb4, 0x4c, 0x88, 0xbe, 0xb6, 0x8f, 0xae, 0xa8,
0xd9, 0xf7, 0x18, 0xc3, 0xa5, 0xd5, 0x00, 0x8c, 0x81, 0x6a, 0x98, 0x2f, 0xab, 0x72, 0x95, 0xe5,
0xa2, 0x22, 0x27, 0xb7, 0x7a, 0x9c, 0xb1, 0x68, 0xc9, 0x89, 0xaf, 0x62, 0x2b, 0xac, 0x8a, 0xef,
0x33, 0x7a, 0xa7, 0x0b, 0xc6, 0x81, 0x38, 0x57, 0x7e, 0x7e, 0x94, 0xdd, 0x1d, 0x67, 0x3d, 0x5d,
0x0a, 0xec, 0x96, 0xb0, 0xdb, 0xcd, 0x63, 0xa7, 0xe4, 0xbf, 0x3e, 0x3a, 0x98, 0xe9, 0x4b, 0xf2,
0x9f, 0xf7, 0xa0, 0x28, 0x76, 0x23, 0x18, 0x13, 0x26, 0xfa, 0x6d, 0x0c, 0xcf, 0xc0, 0x41, 0x82,
0x74, 0x08, 0x32, 0x73, 0xad, 0x20, 0x48, 0xe8, 0x8e, 0xb5, 0xb2, 0xac, 0x4d, 0xe4, 0x32, 0xbb,
0xad, 0x5e, 0xd2, 0xad, 0x91, 0xb4, 0x4e, 0x88, 0x60, 0xbb, 0x08, 0x51, 0x48, 0xdc, 0xdb, 0x1e,
0xf0, 0x72, 0xb2, 0x1c, 0x71, 0x62, 0x62, 0x2c, 0xf1, 0x3d, 0x4e, 0x46, 0x61, 0x3b, 0x87, 0x16,
0x85, 0x9a, 0x46, 0xf2, 0xe3, 0xa1, 0x8f, 0xa9, 0xf2, 0x3e, 0x32, 0x33, 0x80, 0xc0, 0x8b, 0x1c,
0x93, 0x09, 0xf0, 0xfb, 0xbe, 0x68, 0xdf, 0x3f, 0x9a, 0xaf, 0x08, 0x77, 0xe1, 0x18, 0xea, 0xf5,
0xbd, 0x3a, 0xc2, 0x16, 0x3b, 0xc2, 0x42, 0x8f, 0xdd, 0x84, 0x03, 0x04, 0xa8, 0xf5, 0xe5, 0x65,
0x93, 0xd5, 0x3d, 0xce, 0x91, 0xe2, 0xd9, 0x21, 0x98, 0x0d, 0x47, 0x5c, 0x2b, 0x4a, 0x2e, 0x79,
0x95, 0x30, 0xc3, 0xb4, 0xec, 0x03, 0x90, 0x87, 0x81, 0x63, 0x55, 0x53, 0x1b, 0x47, 0xc9, 0xc1,
0x91, 0x28, 0x21, 0x9d, 0x3a, 0x39, 0x89, 0x4e, 0xa6, 0xf1, 0x7a, 0xd4, 0xe8, 0xbf, 0xd8, 0x01,
0xfd, 0xb7, 0xd9, 0xb4, 0xa3, 0x3b, 0x70, 0x0d, 0x09, 0xb2, 0xbb, 0x07, 0xa5, 0x54, 0xbf, 0x04,
0x99, 0x69, 0x62, 0x85, 0x33, 0x06, 0x63, 0xd6, 0x11, 0xa3, 0xdc, 0xeb, 0x59, 0x7e, 0x33, 0xb7,
0x9f, 0xbf, 0xc5, 0x68, 0xfd, 0x9c, 0x77, 0x22, 0x9c, 0xad, 0xf8, 0x93, 0x7d, 0x68, 0x21, 0xca,
0x20, 0x14, 0xa2, 0x3f, 0x7e, 0x3f, 0x92, 0x67, 0x7e, 0x8b, 0x32, 0xb0, 0x8e, 0xbf, 0xad, 0xb3,
0x68, 0xa1, 0x4b, 0xc9, 0x17, 0xd8, 0xc9, 0x2c, 0xa9, 0x3c, 0x36, 0x95, 0xe2, 0x96, 0x42, 0x0b,
0xdc, 0x28, 0x2b, 0xbf, 0x9e, 0x50, 0xa3, 0x96, 0x81, 0x96, 0x1b, 0x89, 0x1d, 0x44, 0xde, 0xae,
0x4f, 0xa8, 0xe2, 0x68, 0x26, 0x61, 0x2b, 0x75, 0x81, 0x08, 0xe4, 0xa5, 0x99, 0x61, 0xcc, 0xc7,
0x2d, 0x79, 0xeb, 0xc3, 0x1b, 0x84, 0x88, 0xf9, 0x8b, 0x28, 0x88, 0xe0, 0x09, 0x6c, 0x40, 0x78,
0xb3, 0xeb, 0xee, 0x3c, 0x5e, 0xd5, 0xc5, 0x31, 0x68, 0xb1, 0x40, 0xd1, 0x6a, 0x99, 0xc0, 0x36,
0xd0, 0x15, 0x84, 0x01, 0x87, 0x0d, 0x58, 0x56, 0x42, 0x40, 0x9c, 0xbb, 0x64, 0x6c, 0xaa, 0x63,
0xd5, 0x20, 0xc8, 0x26, 0xaa, 0x3b, 0x79, 0xdc, 0xd9, 0x38, 0xb1, 0x6b, 0x92, 0x91, 0x34, 0x6e,
0x29, 0x9b, 0xa7, 0x63, 0xc1, 0x15, 0xb8, 0xf0, 0xc9, 0xa5, 0x0f, 0x9f, 0x4f, 0xce, 0x86, 0x90,
0xa7, 0x36, 0x3f, 0x93, 0xf8, 0x8a, 0xc5, 0xa7, 0xcd, 0x3f, 0x23, 0x16, 0xd0, 0x56, 0x83, 0xc8,
0x64, 0x14, 0x2b, 0x33, 0x2b, 0x89, 0xec, 0x57, 0xc1, 0x01, 0x5b, 0x7f, 0x8d, 0x44, 0x48, 0xe7,
0xe2, 0xed, 0xfc, 0x21, 0x75, 0x15, 0x78, 0x10, 0x66, 0xcd, 0x56, 0xc5, 0x18, 0x99, 0x13, 0x75,
0xc0, 0x30, 0xbb, 0xe6, 0x50, 0x86, 0xff, 0x2a, 0xb0, 0x86, 0xad, 0xbf, 0xc9, 0xad, 0x9c, 0xd0,
0x4a, 0xab, 0x52, 0xb7, 0xdc, 0x2d, 0x6e, 0xd0, 0x1b, 0x32, 0x7a, 0x48, 0x7d, 0x88, 0x60, 0x07,
0xf4, 0x41, 0x45, 0xbd, 0x04, 0x4c, 0xc8, 0x2c, 0x7b, 0x66, 0xbd, 0xc3, 0x21, 0xf2, 0x8d, 0x8b,
0x77, 0x2e, 0xf0, 0x7d, 0x19, 0xf6, 0x60, 0x76, 0x87, 0x0e, 0x0e, 0xb9, 0x04, 0x1b, 0x4c, 0x16,
0xa3, 0xfd, 0x3f, 0x9a, 0x14, 0x4b, 0x99, 0x48, 0x0f, 0xff, 0xa9, 0xec, 0x60, 0xf4, 0xa3, 0x7e,
0x43, 0xec, 0x3f, 0x48, 0xee, 0x7b, 0xa7, 0x8b, 0x46, 0x82, 0x1f, 0x5f, 0xd9, 0xf2, 0x96, 0x2c,
0xe7, 0xd7, 0xd4, 0x6f, 0x53, 0xba, 0x00, 0xe9, 0x2c, 0xbc, 0x52, 0x2f, 0x5d, 0x90, 0xa8, 0xc6,
0x77, 0x3c, 0x7d, 0xa3, 0x70, 0x6e, 0x40, 0x35, 0x9a, 0xbe, 0x92, 0x62, 0x98, 0x7e, 0xe0, 0x72,
0x89, 0xc9, 0xbe, 0x84, 0xbb, 0x69, 0x29, 0x27, 0xdc, 0xe2, 0x84, 0x40, 0x3e, 0x96, 0x27, 0x5a,
0x25, 0x60, 0x2a, 0xe3, 0x42, 0xef, 0x17, 0xa9, 0xff, 0xe4, 0x57, 0x52, 0x2d, 0xb6, 0xf4, 0x24,
0xf2, 0xf4, 0x32, 0x4e, 0xf7, 0x08, 0x86, 0xb1, 0x84, 0x59, 0x5c, 0x44, 0xe9, 0xe3, 0x7a, 0xb4,
0x27, 0xbc, 0x57, 0x27, 0xcd, 0xc7, 0xc2, 0x6e, 0xef, 0xae, 0x89, 0x11, 0x84, 0xa1, 0xdf, 0xaa,
0x01, 0xc3, 0x68, 0x3f, 0x0b, 0x3f, 0x70, 0x4c, 0x77, 0xc1, 0x4a, 0x86, 0xba, 0x76, 0xfb, 0x53,
0xad, 0x90, 0x2d, 0x09, 0x92, 0xb9, 0x58, 0xf0, 0x48, 0x66, 0xb0, 0xfa, 0x6b, 0x7a, 0xad, 0x49,
0x83, 0xb3, 0x78, 0x4e, 0xa0, 0x71, 0xeb, 0xe2, 0x15, 0xed, 0xa7, 0xe5, 0x6b, 0xdd, 0x3f, 0x20,
0xe7, 0x48, 0x1f, 0x13, 0x49, 0x91, 0x22, 0xb6, 0x53, 0x6e, 0x9e, 0x69, 0xae, 0x37, 0x0f, 0xa8,
0x0d, 0x36, 0x58, 0x09, 0xc2, 0x83, 0x08, 0x98, 0x54, 0xeb, 0x6a, 0x59, 0x49, 0x9c, 0x34, 0x8c,
0xf8, 0x8f, 0x51, 0x2c, 0xbf, 0x04, 0x85, 0x2e, 0x46, 0xbe, 0x64, 0x5b, 0xe7, 0x86, 0x64, 0x98,
0x9c, 0x20, 0xbd, 0xf5, 0x07, 0x83, 0x54, 0x1e, 0x85, 0xa8, 0xdb, 0x0c, 0xd4, 0x2f, 0x2b, 0x4c,
0xda, 0x6f, 0xbd, 0xda, 0xde, 0xb5, 0x6e, 0xb7, 0xce, 0x88, 0xd8, 0x74, 0x23, 0xfe, 0x7c, 0x5a,
0x06, 0x6e, 0x95, 0x10, 0x95, 0xed, 0x06, 0x24, 0xab, 0x87, 0xee, 0x32, 0x20, 0x52, 0x9f, 0xc0,
0x90, 0x18, 0xb6, 0x20, 0x32, 0xa2, 0xec, 0x1a, 0x6e, 0x4a, 0xb6, 0x27, 0x5a, 0xd8, 0xfb, 0xa4,
0xf6, 0xe6, 0xc5, 0xb2, 0xb4, 0x88, 0x5d, 0x65, 0x7d, 0xb6, 0xf8, 0xc5, 0xc8, 0xd3, 0x84, 0x43,
0x0e, 0x16, 0xb3, 0x34, 0xb5, 0x4f, 0xbd, 0x33, 0xc7, 0x08, 0xd8, 0xbc, 0xf6, 0xcb, 0xf1, 0xe0,
0xb1, 0x5b, 0x87, 0x3f, 0xfb, 0x20, 0x4b, 0x8c, 0x07, 0xd0, 0x17, 0x55, 0x28, 0xd8, 0xdd, 0xe9,
0x4e, 0x04, 0xa2, 0xe0, 0xa1, 0xe8, 0x09, 0x93, 0x9d, 0x49, 0x97, 0xb3, 0x4d, 0x4b, 0x2b, 0xed,
0xf9, 0x52, 0xd4, 0x8a, 0x25, 0x03, 0x65, 0xe9, 0x49, 0xc0, 0x5d, 0xd9, 0x91, 0xc2, 0xaa, 0x1f,
0xe1, 0xb8, 0x90, 0xae, 0xf5, 0x5f, 0x71, 0x16, 0x23, 0x9d, 0x49, 0xd6, 0xec, 0x8b, 0xfa, 0x14,
0x1c, 0xf8, 0x74, 0x23, 0xec, 0x52, 0x05, 0x10, 0xe4, 0x70, 0x66, 0x09, 0x78, 0x53, 0xa9, 0x09,
0x67, 0x3a, 0x53, 0xc9, 0x35, 0x1b, 0x19, 0xb2, 0x53, 0xd0, 0x2c, 0xd5, 0x68, 0x2b, 0xbe, 0x95,
0x93, 0x69, 0xca, 0xe8, 0xf3, 0x01, 0x8a, 0xbb, 0x99, 0x17, 0x4b, 0x91, 0x29, 0x6e, 0x58, 0x89,
0xb0, 0xa7, 0xd8, 0x21, 0x1d, 0x35, 0x48, 0xeb, 0xe5, 0x86, 0x86, 0x33, 0x07, 0xd9, 0x70, 0x8a,
0xca, 0xe1, 0xf1, 0x87, 0x48, 0xa9, 0xab, 0x46, 0x90, 0x38, 0x5e, 0x80, 0x99, 0x22, 0x41, 0xb0,
0xfa, 0xcd, 0xb8, 0xe1, 0xe7, 0x06, 0x50, 0xd9, 0x62, 0x46, 0x26, 0xbe, 0xe3, 0xe5, 0xb8, 0xd9,
0xb7, 0x53, 0x3c, 0x9e, 0x57, 0x64, 0x0d, 0xd9, 0x10, 0x0e, 0xc7, 0xc5, 0xab, 0x5c, 0x31, 0x84,
0xcd, 0x59, 0x75, 0x98, 0xff, 0x85, 0x03, 0x03, 0xf4, 0xb6, 0x84, 0xfc, 0x8b, 0xad, 0xc7, 0x76,
0xe5, 0x6c, 0xe6, 0x63, 0xc3, 0x88, 0xc7, 0xf7, 0xbc, 0x9c, 0xee, 0x10, 0x00, 0x4c, 0xad, 0x6f,
0x7e, 0xe5, 0x81, 0xb2, 0x2c, 0xe5, 0x0a, 0x76, 0x81, 0x1c, 0x37, 0x56, 0x56, 0x63, 0xce, 0x09,
0x47, 0xcd, 0x88, 0x4d, 0xde, 0xfc, 0x3e, 0x41, 0x2b, 0xa6, 0x06, 0x41, 0xb5, 0xde, 0xd0, 0xdc,
0x05, 0x25, 0x25, 0xed, 0xad, 0xd9, 0x39, 0x11, 0x71, 0xe5, 0xbb, 0xba, 0x05, 0xf3, 0xe9, 0xe4,
0xa8, 0x19, 0x6d, 0x80, 0xe5, 0x3e, 0xe4, 0x97, 0xd8, 0x08, 0xa9, 0x9f, 0x54, 0x82, 0x10, 0xff,
0xf1, 0x28, 0xc3, 0x79, 0xf1, 0x4d, 0x74, 0x60, 0x69, 0xee, 0xe8, 0xdf, 0x8e, 0xb6, 0x0c, 0x70,
0x5b, 0xb8, 0x4f, 0x48, 0x18, 0x47, 0xad, 0xd4, 0xeb, 0xf3, 0x66, 0x61, 0x11, 0x99, 0xb4, 0xb8,
0x2b, 0x80, 0x9c, 0xa8, 0x60, 0x2c, 0x03, 0x37, 0xbb, 0x8b, 0xb6, 0x96, 0x0b, 0xcd, 0xa4, 0x7a,
0xc5, 0xf3, 0xdd, 0xb8, 0xc8, 0x8b, 0xdd, 0x47, 0x25, 0x05, 0x44, 0x19, 0xa0, 0x2d, 0x38, 0x43,
0xb1, 0x13, 0x64, 0x8b, 0x65, 0x7b, 0x7c, 0xa7, 0x10, 0x33, 0x8e, 0x73, 0xb4, 0x27, 0xfc, 0x49,
0x9c, 0xba, 0x5e, 0xa0, 0xc2, 0xfd, 0x51, 0x56, 0x76, 0x56, 0x47, 0x87, 0x53, 0x85, 0x28, 0xac,
0x0d, 0xb7, 0x03, 0x17, 0xe7, 0x51, 0xeb, 0x7b, 0xa7, 0x67, 0xf5, 0xb0, 0x95, 0x93, 0xb8, 0xc3,
0x28, 0xb7, 0x15, 0xbe, 0x70, 0x5e, 0x0c, 0x57, 0x0f, 0xef, 0x09, 0x56, 0x43, 0xc5, 0x31, 0xa4,
0xc5, 0xe1, 0xc4, 0x5d, 0x16, 0x87, 0x1f, 0xf9, 0x3f, 0x33, 0x3a, 0x09, 0x2b, 0x55, 0x40, 0x07,
0x65, 0xb0, 0xf6, 0xbb, 0xee, 0x69, 0xa7, 0xc4, 0xd7, 0x07, 0xb1, 0x15, 0x86, 0x00, 0x2f, 0xf2,
0xee, 0x81, 0x9d, 0x5d, 0x82, 0xa6, 0xf1, 0xef, 0x0c, 0xde, 0x83, 0xc3, 0x1b, 0x71, 0x77, 0x0b,
0x6b, 0x40, 0x1a, 0x7c, 0xae, 0xd5, 0x3e, 0x5d, 0x74, 0xb5, 0x9c, 0xa6, 0xd7, 0x9d, 0xbe, 0x4c,
0xa1, 0x86, 0x0f, 0x9c, 0x24, 0x37, 0xf7, 0x7f, 0xba, 0x1b, 0x32, 0x28, 0x7a, 0x2c, 0x7c, 0x2f,
0x6b, 0xd5, 0xba, 0x0c, 0x7f, 0x47, 0x32, 0x1a, 0xc5, 0xae, 0x11, 0x12, 0x51, 0x9a, 0x35, 0x38,
0xb7, 0xfb, 0x6d, 0x40, 0x6f, 0x0a, 0x2e, 0xc6, 0xc8, 0x92, 0xc0, 0xb5, 0xee, 0x4e, 0x6e, 0x4d,
0x96, 0xfd, 0x8f, 0xf4, 0x84, 0x40, 0x48, 0xc8, 0x69, 0xd8, 0xaa, 0x2e, 0x70, 0x90, 0x30, 0xb8,
0x92, 0x94, 0x03, 0xd6, 0x1b, 0xd6, 0x06, 0x20, 0x06, 0x85, 0x38, 0x60, 0x2a, 0x8c, 0x2f, 0xa4,
0xb0, 0x2c, 0x96, 0x63, 0x37, 0x0b, 0xa9, 0x4a, 0x2e, 0xbe, 0x55, 0xcb, 0x73, 0x70, 0xe2, 0xc4,
0xa6, 0xc4, 0x60, 0xce, 0x73, 0x14, 0x74, 0x47, 0x5b, 0x42, 0x0a, 0xa6, 0xe8, 0x20, 0x68, 0x1c,
0x94, 0x20, 0x8e, 0x6c, 0x2d, 0x15, 0x93, 0x40, 0x40, 0xc6, 0x43, 0x95, 0xaf, 0x41, 0x46, 0x04,
0x9b, 0x76, 0x87, 0x7d, 0x16, 0x5c, 0x20, 0xe6, 0xb5, 0x63, 0xb6, 0xb0, 0x74, 0xc6, 0x3c, 0x25,
0xca, 0xae, 0xa6, 0x78, 0xb6, 0xaa, 0x36, 0x0f, 0x83, 0x60, 0x2b, 0x6e, 0x2e, 0x90, 0x91, 0xa6,
0x28, 0x04, 0x36, 0x63, 0x4c, 0x0c, 0x8e, 0x58, 0xd3, 0xd4, 0x35, 0x2d, 0x21, 0xb4, 0x3c, 0x76,
0x38, 0x04, 0x03, 0x02, 0x5c, 0xfa, 0x73, 0xc4, 0xc7, 0xe9, 0x3d, 0x6b, 0xd8, 0x43, 0x8c, 0xe2,
0x73, 0xa1, 0x27, 0xe5, 0xc0, 0x09, 0xf1, 0x90, 0xb0, 0x3c, 0x14, 0xad, 0x46, 0x0b, 0x90, 0xc2,
0x04, 0x67, 0x2f, 0xbc, 0xc8, 0xaf, 0x9a, 0xa8, 0x76, 0x09, 0x51, 0xd9, 0xa7, 0xc1, 0xed, 0x65,
0x15, 0x87, 0xd1, 0x8e, 0x49, 0x0a, 0xaa, 0x48, 0xa0, 0xc2, 0x53, 0x7a, 0x36, 0x52, 0xb2, 0xbd,
0x6f, 0x44, 0x4b, 0xac, 0x2d, 0x00, 0xa0, 0x7f, 0x33, 0xdd, 0x63, 0x45, 0x9f, 0xc9, 0x8e, 0x10,
0xcb, 0xb0, 0x41, 0x5e, 0x44, 0x82, 0x8c, 0x0d, 0xe1, 0xf9, 0x8b, 0xab, 0x5b, 0x02, 0x46, 0xfa,
0xc6, 0x91, 0xdf, 0x73, 0x6b, 0xa4, 0x96, 0x93, 0x62, 0xdb, 0xad, 0xf0, 0x54, 0xcc, 0x1d, 0x21,
0x17, 0x02, 0x2a, 0x88, 0x23, 0x13, 0xa1, 0xa0, 0x7f, 0xcc, 0xd5, 0xa8, 0x52, 0xa4, 0x81, 0x44,
0x41, 0xb2, 0x71, 0x6e, 0xcf, 0xef, 0xc6, 0xce, 0x34, 0x49, 0x6e, 0x4d, 0xad, 0xaa, 0xf5, 0xbb,
0xe2, 0x6a, 0xa7, 0x24, 0x88, 0xb5, 0xed, 0x92, 0xdc, 0xb4, 0x19, 0x58, 0xc5, 0x96, 0x98, 0x5c,
0xb5, 0x4a, 0x2d, 0x45, 0xcd, 0xe5, 0xe8, 0xb3, 0x0c, 0x8d, 0xff, 0xb8, 0xbd, 0x02, 0xcf, 0x64,
0x42, 0x40, 0xa1, 0xa3, 0x59, 0x35, 0x23, 0xe8, 0xfe, 0x99, 0xa0, 0x38, 0xfb, 0x7e, 0xfc, 0x78,
0x3a, 0x43, 0xa5, 0xd7, 0x2a, 0x0f, 0x02, 0x89, 0x62, 0x31, 0xcb, 0x6e, 0x00, 0xbf, 0x78, 0x1e,
0x66, 0x0b, 0x51, 0xe4, 0x43, 0x7f, 0x8c, 0x11, 0x39, 0x18, 0x9d, 0xb7, 0xf5, 0x67, 0x4e, 0x7f,
0x81, 0xb8, 0x79, 0x4e, 0x1b, 0xdd, 0x8f, 0x65, 0x65, 0x66, 0x19, 0x06, 0x56, 0xf0, 0x54, 0xb5,
0x7b, 0xdf, 0x05, 0x36, 0x5f, 0x31, 0xc1, 0x3a, 0x6b, 0x11, 0x74, 0xe1, 0xe5, 0xcd, 0xc0, 0x70,
0x9d, 0xf5, 0xfa, 0x30, 0x37, 0x1a, 0xa8, 0x31, 0xff, 0xe9, 0x49, 0xc9, 0xd4, 0x6f, 0x5f, 0xac,
0x31, 0x2c, 0x95, 0x67, 0x0d, 0x18, 0xfa, 0xdd, 0x0a, 0xef, 0xbd, 0x09, 0xe9, 0x30, 0x51, 0xe0,
0xcd, 0x12, 0x55, 0xb1, 0x09, 0xe5, 0x43, 0x5b, 0xff, 0xce, 0xd8, 0xa3, 0x33, 0x56, 0xd2, 0xbc,
0x67, 0xd3, 0x7d, 0x98, 0x00, 0x8a, 0x9d, 0x8d, 0xcc, 0xa8, 0x0a, 0xb4, 0x92, 0x06, 0x52, 0x33,
0xf7, 0xda, 0xad, 0x73, 0xbb, 0xb7, 0xa0, 0xd8, 0xd1, 0xb4, 0xc4, 0xf4, 0xee, 0xb7, 0x7a, 0x06,
0xea, 0xda, 0x7a, 0x04, 0xeb, 0x0d, 0x84, 0xa5, 0xc4, 0x11, 0xde, 0xf4, 0xd6, 0xb5, 0xac, 0x85,
0x54, 0xe5, 0x16, 0xcf, 0x97, 0x3c, 0xe0, 0x2f, 0x58, 0x1b, 0x11, 0xa2, 0xfd, 0x8c, 0xe1, 0x1a,
0x12, 0x34, 0x19, 0x44, 0x99, 0x9a, 0x39, 0xa9, 0xf9, 0x25, 0x34, 0x76, 0xbe, 0xe1, 0x22, 0x48,
0x93, 0x1d, 0xa3, 0x38, 0x3c, 0x6b, 0xc7, 0xa7, 0xc6, 0x14, 0xb8, 0x9d, 0x6c, 0x1e, 0x35, 0x2a,
0x0b, 0x77, 0xb3, 0x82, 0x53, 0x6d, 0x4c, 0x15, 0xd8, 0x16, 0xd1, 0x5d, 0xc8, 0x9b, 0x45, 0x24,
0xda, 0x2c, 0xac, 0xb5, 0xfa, 0x18, 0x15, 0xeb, 0x60, 0xd2, 0x4f, 0xd8, 0xb9, 0x93, 0xcd, 0x2c,
0xf4, 0x03, 0xa1, 0xcd, 0x8a, 0xef, 0x42, 0x25, 0xfc, 0xa9, 0x85, 0xf3, 0xa5, 0x4a, 0xb7, 0x5b,
0x9c, 0x01, 0x2e, 0x31, 0x8c, 0x29, 0x46, 0xdc, 0x5d, 0x09, 0xf8, 0x5c, 0xdf, 0xba, 0x8a, 0x00,
0xdc, 0x8e, 0x81, 0xb9, 0x09, 0x34, 0x22, 0x3a, 0x4a, 0x54, 0xe9, 0xfc, 0x6a, 0xca, 0x0d, 0xdf,
0xaa, 0xc8, 0xa5, 0x3f, 0x0d, 0x9b, 0x7a, 0xc8, 0x2c, 0xa8, 0x8f, 0xf3, 0x84, 0x4c, 0x6f, 0x14,
0x1c, 0xe7, 0xea, 0x56, 0xa0, 0x03, 0x91, 0xb7, 0xe0, 0x2b, 0x72, 0xea, 0x3e, 0x4d, 0x5a, 0x9a,
0x39, 0x30, 0x20, 0x90, 0xa6, 0x2f, 0x5f, 0xcb, 0x9c, 0x60, 0x4d, 0x98, 0xc1, 0x01, 0x0d, 0x4b,
0x60, 0xf9, 0x2b, 0xfc, 0x16, 0x3f, 0x48, 0xcf, 0xb4, 0xa6, 0xc8, 0xfb, 0xea, 0x67, 0x88, 0xab,
0xf2, 0x24, 0x0b, 0x6c, 0x31, 0xce, 0xab, 0x06, 0x7c, 0xc0, 0xb5, 0xc8, 0x17, 0x59, 0x56, 0x06,
0xca, 0xb0, 0x6d, 0x6d, 0x49, 0xf1, 0x42, 0x37, 0x49, 0x10, 0x3b, 0x99, 0x76, 0x3a, 0x03, 0xcf,
0x0d, 0x1c, 0xc3, 0x27, 0x9d, 0xea, 0x46, 0x4e, 0xf5, 0x80, 0xb5, 0xcc, 0x7b, 0x1f, 0xca, 0xf2,
0x81, 0x39, 0xc6, 0xcf, 0xcb, 0xdc, 0x39, 0x76, 0x60, 0x75, 0xd1, 0xe6, 0x9d, 0x2c, 0x53, 0x1a,
0x58, 0xb7, 0x2c, 0xa4, 0x0b, 0xf1, 0x8f, 0xb8, 0x96, 0x8f, 0xf5, 0xa7, 0xa8, 0x00, 0xdd, 0x1a,
0xcd, 0xdc, 0xa8, 0xfd, 0x86, 0x5d, 0x31, 0xe6, 0x8d, 0xc9, 0xa1, 0x07, 0x04, 0xe1, 0x4b, 0x93,
0x2d, 0x00, 0xf7, 0x6e, 0x84, 0x65, 0x6d, 0x18, 0x48, 0x09, 0x26, 0x2d, 0xda, 0x59, 0x93, 0x03,
0x95, 0x50, 0x9c, 0x7e, 0x4c, 0x3c, 0xe4, 0x05, 0x94, 0x3c, 0x4f, 0x6b, 0x1b, 0xdc, 0x67, 0x98,
0x9e, 0x1c, 0xa0, 0xc0, 0xca, 0x23, 0x6f, 0x64, 0x34, 0x7a, 0x98, 0x4f, 0x35, 0xbc, 0xb5, 0x0f,
0x8c, 0xe6, 0xba, 0x34, 0xfc, 0x3b, 0x40, 0xbc, 0xb9, 0xb4, 0xb5, 0xc7, 0x9b, 0x37, 0x8a, 0xca,
0xed, 0xfd, 0x3f, 0x0a, 0xe7, 0xb2, 0x73, 0xe8, 0x7b, 0x83, 0xc8, 0x66, 0xc2, 0x5f, 0xc7, 0xf0,
0xe7, 0x50, 0x27, 0x1f, 0xe4, 0xf7, 0xd9, 0x15, 0x98, 0x11, 0xa7, 0x93, 0x6d, 0x51, 0x8c, 0x2f,
0x96, 0xc8, 0x7a, 0xf2, 0xf7, 0x55, 0x8b, 0x01, 0x89, 0xaf, 0x99, 0x8e, 0x83, 0xd4, 0xd4, 0x97,
0x96, 0xe3, 0x98, 0xed, 0x53, 0xba, 0x4f, 0xb6, 0xb2, 0x72, 0x5a, 0x1d, 0x66, 0x60, 0x63, 0x54,
0xe1, 0xbb, 0xa9, 0x52, 0x16, 0x57, 0x18, 0x56, 0x44, 0x25, 0x80, 0xfe, 0x90, 0xcf, 0xcb, 0xd0,
0xaa, 0x4a, 0x4a, 0x6e, 0x71, 0xe6, 0x80, 0xd4, 0x40, 0x07, 0xed, 0xdc, 0x3d, 0xbb, 0xbb, 0xad,
0x91, 0x1a, 0x69, 0x63, 0x85, 0x25, 0xb0, 0x1c, 0x4c, 0x47, 0xb0, 0x1f, 0xf7, 0x3b, 0x32, 0x58,
0xa3, 0xe8, 0x96, 0xa6, 0xe7, 0xe2, 0xb6, 0x48, 0x7c, 0xd8, 0x70, 0x74, 0xac, 0x32, 0xb8, 0x08,
0x21, 0xe6, 0x59, 0x03, 0xcc, 0xe7, 0x5e, 0xaa, 0xf5, 0xa2, 0xd5, 0xdf, 0x39, 0x8e, 0x00, 0xc8,
0x18, 0x9f, 0x70, 0xc3, 0xd4, 0x09, 0x19, 0xf3, 0x87, 0xa6, 0x0a, 0xf9, 0x36, 0x3d, 0x40, 0x90,
0xe6, 0x34, 0x0d, 0xe2, 0x82, 0xd9, 0x76, 0x18, 0x68, 0x12, 0x28, 0x9d, 0xc8, 0xae, 0x6f, 0xbe,
0xe9, 0x6e, 0x7d, 0x33, 0x7e, 0xc7, 0x10, 0x09, 0x7e, 0x54, 0xfd, 0xc1, 0xb8, 0xbb, 0x5d, 0x20,
0xfa, 0xc2, 0x8c, 0x5a, 0xda, 0x72, 0xe4, 0x89, 0x0d, 0x24, 0x56, 0x74, 0xd7, 0xf5, 0xcc, 0xc9,
0x88, 0x0d, 0xe4, 0xf6, 0x54, 0xfb, 0x9f, 0xfc, 0x45, 0xd6, 0x98, 0xf1, 0xe6, 0x78, 0xab, 0x69,
0xb8, 0x95, 0xb8, 0xd7, 0xfd, 0x06, 0xcc, 0x2c, 0x46, 0xa3, 0x72, 0x5f, 0xf0, 0x2a, 0x09, 0x3c,
0xc0, 0xee, 0x93, 0xcf, 0xaa, 0x4a, 0xa5, 0xe7, 0x38, 0x83, 0xcb, 0xdd, 0xce, 0xa5, 0xf8, 0x75,
0x44, 0x7f, 0x62, 0x42, 0x6c, 0xb3, 0x3f, 0x64, 0xf4, 0xfa, 0x00, 0x3d, 0xdc, 0xf3, 0xb2, 0x2e,
0xe1, 0x9d, 0xf4, 0x17, 0x5d, 0x0f, 0x05, 0xf6, 0xa2, 0xce, 0x8a, 0xbf, 0x8c, 0x73, 0x43, 0xfa,
0xce, 0xf7, 0x01, 0x4c, 0x2c, 0x1b, 0x6e, 0xff, 0x96, 0x7f, 0x7c, 0x5e, 0xb8, 0x5f, 0xd5, 0x18,
0x88, 0x89, 0x67, 0x14, 0xe9, 0xdb, 0x31, 0x67, 0x06, 0x05, 0xb8, 0x75, 0x97, 0x3c, 0xce, 0xf3,
0x72, 0xbd, 0xb3, 0x6c, 0xd1, 0x7e, 0x4a, 0xc8, 0xa2, 0x2c, 0xa0, 0x88, 0x12, 0xc0, 0x27, 0xe8,
0x6f, 0xa1, 0xc6, 0x49, 0x05, 0xe2, 0xf1, 0x81, 0x2b, 0x8a, 0x3e, 0xc3, 0x23, 0x48, 0x58, 0xbb,
0x21, 0xa1, 0xad, 0x7b, 0x9a, 0x03, 0xcd, 0xb0, 0x00, 0xd0, 0xe6, 0xe4, 0x82, 0x7b, 0xe8, 0x70,
0x33, 0xe9, 0x4b, 0xe0, 0x3c, 0x1f, 0x57, 0x10, 0x9e, 0xfa, 0x8a, 0x3a, 0xcd, 0x36, 0x49, 0x09,
0x62, 0xbd, 0x5b, 0x97, 0xda, 0x2e, 0xfb, 0x57, 0xe2, 0x53, 0x9b, 0xbf, 0xe3, 0x66, 0x48, 0x59,
0xc4, 0x22, 0xa9, 0x92, 0xf1, 0x38, 0xe2, 0xab, 0xd3, 0xb6, 0xb9, 0x4f, 0xc0, 0x6b, 0x6e, 0x26,
0xb5, 0xa6, 0x0a, 0xce, 0x0f, 0xfc, 0x54, 0x98, 0xda, 0x59, 0x68, 0xd4, 0x64, 0xa4, 0xc2, 0x5a,
0xa0, 0x95, 0x93, 0xef, 0x87, 0xc5, 0x5e, 0x73, 0xab, 0xce, 0xa9, 0xe6, 0x3c, 0x67, 0x51, 0xeb,
0xee, 0x0d, 0xd9, 0x66, 0x77, 0x4b, 0x57, 0x1f, 0xb3, 0x46, 0xde, 0x52, 0x2b, 0x84, 0xb9, 0x52,
0x2e, 0x1a, 0x4d, 0xf9, 0x46, 0x0d, 0x77, 0xd1, 0x5a, 0x08, 0x5e, 0x3c, 0xfe, 0x7c, 0x3e, 0x4f,
0x9c, 0x52, 0x0c, 0xe3, 0xeb, 0x83, 0x44, 0x08, 0x98, 0xbb, 0x10, 0x70, 0x40, 0xb9, 0x71, 0xde,
0x4d, 0xe2, 0x7f, 0x90, 0xc3, 0x83, 0x1f, 0xa4, 0x73, 0x36, 0x8c, 0x94, 0x3f, 0x5b, 0x8d, 0x85,
0xa9, 0x2e, 0x49, 0x2d, 0x3e, 0x5a, 0xda, 0x70, 0xe1, 0x6d, 0xa5, 0x52, 0x33, 0x94, 0xfb, 0x26,
0x28, 0xc3, 0xe2, 0xc8, 0xce, 0x78, 0x35, 0x19, 0xa3, 0xee, 0x39, 0xdd, 0x78, 0x04, 0xa3, 0x1e,
0x73, 0x82, 0xc0, 0x73, 0x24, 0xb6, 0xbc, 0xd0, 0xa1, 0x50, 0x64, 0xb3, 0x24, 0xb6, 0xf3, 0x58,
0x88, 0x22, 0xcf, 0x52, 0xc0, 0xca, 0x70, 0xa0, 0x88, 0x6e, 0xac, 0xdf, 0x49, 0xb3, 0xd5, 0x05,
0xb1, 0xbe, 0xfa, 0xad, 0x9e, 0x30, 0xb6, 0xce, 0xd9, 0x5a, 0x66, 0xa6, 0x23, 0xcb, 0x91, 0x8a,
0x00, 0x42, 0x64, 0xfa, 0x72, 0x0a, 0x36, 0xa9, 0x6d, 0x49, 0xca, 0x93, 0xff, 0xd9, 0x37, 0xfe,
0xe9, 0xde, 0x4e, 0x4c, 0xb1, 0x8c, 0x35, 0xa5, 0x48, 0xd2, 0xb6, 0x3a, 0xcb, 0x41, 0x03, 0x5c,
0x2c, 0xfb, 0xba, 0xe7, 0x36, 0x47, 0x53, 0xfd, 0x44, 0xff, 0x47, 0x2b, 0x02, 0xbc, 0xee, 0xd2,
0xbe, 0x50, 0xd3, 0x36, 0xdd, 0xe2, 0x9a, 0xc1, 0xc1, 0x9a, 0xad, 0x5b, 0x8d, 0x9a, 0xb3, 0x7b,
0xc2, 0xd7, 0x7d, 0x66, 0xcd, 0x30, 0x46, 0x0c, 0x89, 0x62, 0x18, 0xf4, 0xc3, 0x04, 0x5d, 0x1f,
0x54, 0x98, 0xd8, 0xa9, 0xb9, 0x97, 0x0b, 0x27, 0xb4, 0xe8, 0x41, 0xa0, 0x2d, 0x71, 0x97, 0x77,
0x7e, 0xe6, 0xdf, 0x30, 0x3b, 0xbe, 0x21, 0xe7, 0x0c, 0x7f, 0x0e, 0x12, 0xc4, 0xc9, 0x84, 0x75,
0x4e, 0x86, 0x84, 0x32, 0x88, 0x58, 0x1a, 0xa9, 0xea, 0x73, 0x61, 0x5c, 0x43, 0x5b, 0x87, 0xa3,
0x97, 0x06, 0x6d, 0x19, 0xea, 0x42, 0x21, 0xb3, 0x98, 0x48, 0x74, 0x92, 0xfc, 0xd8, 0xc1, 0x21,
0x8f, 0x8e, 0x9e, 0xf8, 0x13, 0xb2, 0x4b, 0x28, 0x49, 0xdd, 0x3d, 0x46, 0xf0, 0x2f, 0x9b, 0x99,
0xe3, 0x3f, 0x02, 0x18, 0xf8, 0x1a, 0x95, 0x95, 0x4f, 0x89, 0x09, 0x35, 0x54, 0xcc, 0x15, 0x06,
0x47, 0x06, 0x6c, 0xd6, 0xea, 0x28, 0x3a, 0x1d, 0x4d, 0x3b, 0x4a, 0xc1, 0x60, 0x1a, 0xd2, 0x40,
0xce, 0x1f, 0x56, 0xec, 0x63, 0x9b, 0xbf, 0xb1, 0xbe, 0x5d, 0x4b, 0x80, 0x16, 0x5c, 0x77, 0x2c,
0x27, 0xe2, 0x03, 0x10, 0x81, 0x8e, 0x98, 0x1b, 0xa0, 0xc2, 0x00, 0xfc, 0x8e, 0x29, 0xda, 0x2c,
0xbc, 0x93, 0x87, 0x7f, 0xac, 0x19, 0xa5, 0x13, 0xe5, 0x0f, 0xdc, 0x8f, 0xcd, 0xd8, 0xcd, 0x16,
0xfc, 0x2f, 0x2d, 0x1e, 0xd6, 0x86, 0x4c, 0x0a, 0xde, 0x0c, 0x7e, 0xae, 0x07, 0x6d, 0xda, 0x9d,
0xec, 0x03, 0xdb, 0xb9, 0x77, 0x8a, 0x70, 0x2c, 0x03, 0xaf, 0xbe, 0xd5, 0x34, 0x7c, 0x07, 0xe0,
0x31, 0x69, 0x5b, 0x50, 0x4f, 0xc8, 0x2f, 0x2d, 0xea, 0x42, 0x0a, 0xb2, 0xd2, 0xde, 0xf2, 0x01,
0x93, 0xf1, 0xab, 0x84, 0xa1, 0x59, 0x62, 0xb5, 0x33, 0x17, 0xea, 0xec, 0xbf, 0xb0, 0x03, 0x64,
0x73, 0x53, 0x54, 0xf6, 0xd9, 0x06, 0x56, 0xaa, 0xd3, 0xbd, 0x23, 0x0e, 0x9c, 0xae, 0x80, 0xee,
0x7f, 0xfa, 0x75, 0x39, 0xfa, 0x92, 0x2e, 0x04, 0xdc, 0xfb, 0x3a, 0xd0, 0x8d, 0x75, 0x7d, 0xd1,
0x43, 0x41, 0x81, 0x32, 0xb3, 0xa8, 0x4e, 0xcd, 0x8f, 0xbe, 0xbe, 0x9d, 0x81, 0x12, 0xe2, 0x3a,
0x6e, 0xa0, 0x30, 0xa3, 0x25, 0x01, 0x04, 0xd0, 0x62, 0xc7, 0xa0, 0xd4, 0x4d, 0x1f, 0xf2, 0x23,
0x94, 0x82, 0x70, 0x36, 0xce, 0xdd, 0x3a, 0x84, 0x9d, 0x56, 0xba, 0xf4, 0x04, 0x05, 0x79, 0x13,
0x6a, 0x56, 0x67, 0xf5, 0x58, 0xe5, 0xc5, 0xb4, 0x8d, 0xfb, 0x69, 0xae, 0xe6, 0x0c, 0x8e, 0x71,
0x47, 0xd6, 0xf6, 0xc3, 0x12, 0x68, 0xdf, 0x51, 0x5c, 0x23, 0xfd, 0x11, 0x67, 0xa2, 0x41, 0x13,
0xf8, 0x75, 0x4c, 0x0b, 0x8b, 0x3e, 0xec, 0x54, 0x40, 0x3b, 0x8e, 0x21, 0xcb, 0x82, 0xda, 0xb6,
0xd3, 0xc8, 0x84, 0x37, 0x03, 0x68, 0xc3, 0xda, 0xc0, 0x9a, 0xbc, 0x11, 0x66, 0xba, 0x8d, 0xe4,
0x63, 0x56, 0x77, 0x01, 0x35, 0x31, 0x19, 0xf2, 0xa6, 0x9b, 0x9d, 0xc2, 0xeb, 0xf4, 0x59, 0x5e,
0x9f, 0xb8, 0xd7, 0x8a, 0x85, 0x0d, 0x4c, 0x9a, 0x3f, 0xfd, 0xf1, 0xb0, 0xad, 0x1a, 0x7c, 0xda,
0x54, 0x25, 0x5e, 0x69, 0x0e, 0x03, 0x61, 0x23, 0xce, 0xaf, 0x89, 0x83, 0x0b, 0x0d, 0x7b, 0x4d,
0xe1, 0x51, 0xd3, 0xa5, 0x3a, 0xcc, 0xaa, 0x6e, 0xa9, 0x20, 0x13, 0xe1, 0x7d, 0x65, 0x2e, 0x8a,
0xb9, 0x88, 0xbb, 0x02, 0x11, 0x3c, 0x27, 0xb9, 0x39, 0xbc, 0x88, 0x7f, 0x1a, 0x37, 0x62, 0xcf,
0x02, 0xe5, 0xfb, 0x9e, 0xf8, 0x0a, 0xee, 0xdb, 0xf8, 0x89, 0xc8, 0x53, 0x33, 0x60, 0x4b, 0xc2,
0x3f, 0x45, 0x6e, 0xf8, 0xcf, 0xd1, 0x72, 0x10, 0x9f, 0xdc, 0x43, 0x58, 0x93, 0x81, 0xd5, 0x1d,
0xb3, 0xce, 0x31, 0xdf, 0x14, 0xd8, 0x6b, 0x56, 0xb6, 0x76, 0x27, 0xc5, 0x65, 0x2a, 0x04, 0x57,
0x40, 0x67, 0xa4, 0x3f, 0xbf, 0xb0, 0x02, 0xb4, 0xe9, 0x44, 0x42, 0x25, 0xcd, 0x2b, 0xaf, 0x0c,
0x3d, 0x85, 0x6f, 0x7e, 0x2d, 0xd9, 0xce, 0x1d, 0x75, 0x5b, 0x74, 0xf0, 0x73, 0x61, 0x9f, 0xbb,
0xf6, 0xbc, 0x3f, 0xc6, 0x06, 0xf2, 0x5e, 0xf8, 0x74, 0x53, 0x4f, 0x11, 0xf8, 0x45, 0x5c, 0xea,
0xca, 0xfe, 0xe6, 0xb6, 0xe7, 0xeb, 0x6e, 0xe0, 0xc9, 0x61, 0x7a, 0xd8, 0xb5, 0x4f, 0x56, 0x1c,
0x14, 0xdd, 0xb8, 0xd5, 0xf4, 0x57, 0x34, 0xfa, 0x36, 0xb0, 0xd9, 0x7b, 0x04, 0xe3, 0x29, 0x50,
0x38, 0x16, 0xbf, 0x9d, 0x0f, 0x61, 0xd7, 0x8f, 0x32, 0xd7, 0xfb, 0x30, 0x7d, 0x14, 0xe8, 0x38,
0x44, 0xf6, 0x56, 0x32, 0x29, 0x79, 0xc9, 0xb7, 0x5d, 0x56, 0x89, 0x58, 0x04, 0x61, 0xea, 0xa4,
0x22, 0x9f, 0x9c, 0x01, 0x52, 0x88, 0x0e, 0x9b, 0x2f, 0x31, 0xfe, 0x68, 0x00, 0x1e, 0x94, 0xc7,
0xd5, 0xcc, 0xcf, 0x8c, 0x7d, 0x7a, 0xfa, 0xfb, 0x5a, 0x84, 0x7e, 0x1b, 0xa0, 0xbc, 0xb8, 0x93,
0xaf, 0xb2, 0x52, 0x83, 0x88, 0x77, 0x40, 0x83, 0xea, 0x77, 0xfe, 0xea, 0xe2, 0x5b, 0x98, 0x31,
0x37, 0xaf, 0x30, 0xfc, 0x7b, 0x01, 0x52, 0xbb, 0xae, 0x0d, 0x0e, 0x54, 0x7e, 0xdc, 0xca, 0x06,
0xfb, 0x0c, 0xd4, 0xab, 0xdb, 0x80, 0xa1, 0x66, 0xa5, 0x95, 0x56, 0xbf, 0x13, 0xe0, 0x04, 0x55,
0xcb, 0x39, 0xab, 0xff, 0x1c, 0x05, 0x6e, 0xbc, 0x8b, 0xb6, 0x07, 0x22, 0xcb, 0xf4, 0x23, 0x6a,
0x00, 0x00, 0xb7, 0x8f, 0xba, 0x3e, 0x2b, 0xe8, 0x1e, 0xb4, 0x21, 0xff, 0xc5, 0xe3, 0xb0, 0xff,
0x14, 0x17, 0x17, 0xd3, 0xec, 0x5c, 0xea, 0xf0, 0x9a, 0x1c, 0xd6, 0x03, 0x9a, 0xfb, 0x0f, 0xc0,
0xb5, 0xec, 0xfd, 0x0b, 0xc2, 0x98, 0x39, 0x05, 0xd2, 0xf8, 0xb5, 0xe6, 0xa5, 0xcd, 0xaf, 0xbb,
0x56, 0xa8, 0x80, 0x80, 0x38, 0xe7, 0x02, 0xfc, 0x7c, 0x84, 0x0a, 0x89, 0x68, 0x87, 0x50, 0x4d,
0x33, 0x9b, 0x6e, 0xea, 0x5b, 0x1b, 0xc0, 0x7f, 0x27, 0x9c, 0xfb, 0xfb, 0xdc, 0x3c, 0xb2, 0x37,
0xb0, 0xd7, 0x2f, 0x3c, 0x49, 0xd2, 0x2b, 0xae, 0x08, 0xdc, 0x86, 0xd9, 0x0b, 0xff, 0x00, 0x83,
0x36, 0x91, 0xa1, 0x30, 0x79, 0x4e, 0x01, 0x22, 0x57, 0xf1, 0xde, 0x27, 0x0a, 0x91, 0x4a, 0x42,
0x10, 0x96, 0x03, 0x33, 0x93, 0x20, 0xf2, 0x73, 0x20, 0x42, 0x30, 0x74, 0x4a, 0xae, 0x67, 0x66,
0x94, 0xfc, 0x85, 0xf4, 0x77, 0x4a, 0x87, 0xc9, 0xf1, 0x63, 0x5e, 0xf6, 0x7a, 0x1c, 0x09, 0x56,
0x18, 0x2a, 0xe6, 0x58, 0x08, 0xfc, 0x08, 0x4a, 0x54, 0xf6, 0xde, 0xf2, 0xfe, 0x8b, 0x74, 0x4a,
0xce, 0x4b, 0x62, 0xbb, 0xe2, 0xb6, 0x97, 0x06, 0x47, 0x24, 0x2c, 0xf7, 0x1e, 0x34, 0x38, 0x61,
0xba, 0xa5, 0xac, 0x07, 0xf6, 0x28, 0x1e, 0x71, 0x4c, 0x8c, 0xf6, 0x7b, 0xa3, 0xc7, 0x55, 0xac,
0x0f, 0xef, 0x06, 0x5d, 0x27, 0xa8, 0x2a, 0xde, 0x9a, 0x99, 0xf7, 0xe6, 0x57, 0xdc, 0x71, 0xfc,
0xb8, 0x6a, 0xdd, 0x86, 0xd2, 0x6a, 0xed, 0x49, 0xc4, 0xc4, 0x5b, 0x61, 0xd6, 0xa8, 0xf6, 0xe9,
0xb7, 0xf5, 0xd4, 0x1a, 0xc0, 0xb0, 0x19, 0x40, 0x22, 0xb1, 0xf3, 0x03, 0x5d, 0x29, 0x77, 0x11,
0x83, 0xe4, 0x95, 0xe8, 0xc9, 0x6b, 0x3f, 0x1a, 0x3f, 0xdc, 0x70, 0xa7, 0x2a, 0x29, 0x38, 0x00,
0x62, 0x09, 0x55, 0x4b, 0x07, 0x35, 0xe1, 0xcb, 0x75, 0xc2, 0xe6, 0xdd, 0x1b, 0x93, 0xfe, 0xd3,
0x9c, 0xc5, 0x2c, 0xad, 0x2f, 0x3e, 0x02, 0xd7, 0x7b, 0x12, 0x2d, 0x85, 0x85, 0x13, 0x1b, 0xc5,
0x19, 0x02, 0x57, 0x89, 0xe5, 0x07, 0x57, 0xbc, 0x19, 0x99, 0x62, 0x0d, 0x42, 0x09, 0xbe, 0xfc,
0xeb, 0x15, 0x5c, 0x51, 0x40, 0x26, 0xc2, 0xa7, 0xd9, 0x1c, 0x4c, 0xe8, 0xf8, 0xaa, 0xd9, 0x43,
0x88, 0x16, 0xba, 0xe6, 0xd6, 0x38, 0x68, 0x5c, 0x46, 0x01, 0xad, 0xec, 0x75, 0xba, 0x47, 0x8b,
0x5a, 0x9b, 0x95, 0x3d, 0x48, 0xa1, 0xad, 0x9a, 0xf8, 0x35, 0x9c, 0x0a, 0x7c, 0xa4, 0x7f, 0xe7,
0x9b, 0x82, 0x37, 0x97, 0x06, 0xa1, 0xee, 0xff, 0x66, 0x96, 0x49, 0x9d, 0x74, 0xd0, 0x92, 0x66,
0x78, 0xca, 0xd8, 0x2c, 0xe2, 0xad, 0x39, 0xf4, 0x8b, 0xb4, 0xfd, 0x7b, 0xe1, 0xf0, 0x15, 0x68,
0xc6, 0x32, 0xbf, 0x84, 0x56, 0x24, 0xab, 0x40, 0xbb, 0x4d, 0xe6, 0x97, 0xa8, 0xe3, 0x9d, 0xcd,
0x14, 0x9c, 0xb9, 0x1e, 0xdc, 0xc7, 0xa7, 0x37, 0x93, 0x8b, 0xf3, 0xca, 0x49, 0x21, 0x8d, 0xfc,
0x73, 0xcc, 0x5c, 0xe4, 0x95, 0xb9, 0x04, 0x08, 0x94, 0x63, 0x1f, 0xf8, 0xcd, 0xe8, 0x89, 0x8a,
0xc6, 0x9d, 0x6f, 0xe8, 0x89, 0xa7, 0x43, 0x12, 0xa0, 0x17, 0x9a, 0x5c, 0x78, 0x02, 0xfb, 0xdc,
0x70, 0x5e, 0x5d, 0xfe, 0x7c, 0x9e, 0xff, 0x05, 0x0b, 0xda, 0x50, 0x26, 0x62, 0x85, 0x62, 0xd9,
0x31, 0x31, 0xdc, 0xb7, 0x0a, 0x45, 0xcc, 0x50, 0x01, 0xdf, 0x0b, 0x99, 0xf4, 0xe0, 0x90, 0xf7,
0xb5, 0x0d, 0xec, 0x77, 0x26, 0xd2, 0x08, 0x68, 0x5f, 0xde, 0x0e, 0x2e, 0x84, 0x84, 0x50, 0xd5,
0x6f, 0x7e, 0xd6, 0x85, 0xbe, 0xcc, 0x18, 0x87, 0x9e, 0xe8, 0x4e, 0xcf, 0x06, 0xd6, 0xf5, 0xba,
0x5c, 0x2e, 0xb7, 0x78, 0x4f, 0x44, 0xb2, 0x8e, 0xb2, 0x81, 0x21, 0x8b, 0xb8, 0x70, 0x16, 0x68,
0xb6, 0xa7, 0x58, 0x49, 0xda, 0x8a, 0x89, 0x56, 0xd3, 0x76, 0x8b, 0x3c, 0x9d, 0xd5, 0x54, 0xad,
0xc0, 0x40, 0x90, 0xc2, 0x5c, 0x3f, 0x8d, 0x0e, 0xd3, 0x02, 0x16, 0x60, 0x99, 0x99, 0x43, 0xf0,
0x77, 0x28, 0xfd, 0x02, 0x28, 0xf4, 0x0b, 0xb6, 0xb3, 0xbe, 0x45, 0xf9, 0xe6, 0xaa, 0x7c, 0xb4,
0xdc, 0xf2, 0xfe, 0x7f, 0xb8, 0xe1, 0x87, 0x1c, 0x5a, 0x3f, 0x3d, 0xcf, 0x07, 0xaa, 0x3e, 0xfc,
0xa9, 0x47, 0xc2, 0x77, 0x1f, 0x05, 0x72, 0x42, 0xcc, 0x0a, 0x9a, 0xd4, 0x87, 0x4e, 0x73, 0xe3,
0x06, 0x8e, 0xe5, 0x43, 0xcf, 0x5a, 0xdf, 0xec, 0xd7, 0x3a, 0xfd, 0x23, 0x2b, 0x95, 0x58, 0x15,
0x59, 0x1e, 0xe8, 0x38, 0xea, 0xec, 0xfa, 0xab, 0x70, 0x0b, 0xe1, 0x6a, 0xa6, 0xc7, 0x6a, 0x68,
0xb4, 0x76, 0xf0, 0x82, 0x68, 0x7f, 0x1d, 0x33, 0xcb, 0x33, 0x26, 0xf6, 0x1a, 0xf2, 0xbe, 0xf9,
0x95, 0xdb, 0x37, 0x53, 0x39, 0x35, 0x87, 0x71, 0x0b, 0x6d, 0x66, 0x2a, 0xa5, 0x42, 0x7a, 0x58,
0xbf, 0x7e, 0x14, 0x6f, 0x34, 0x3d, 0x82, 0x38, 0xf8, 0x28, 0x82, 0xf4, 0x6b, 0x2e, 0xf5, 0xc6,
0x3b, 0xbf, 0xca, 0xde, 0xa2, 0xd7, 0xf9, 0x4b, 0x81, 0x9c, 0xcd, 0x7b, 0xc7, 0xe9, 0x53, 0x98,
0x31, 0x88, 0xe3, 0x2e, 0x08, 0x11, 0x1b, 0xdd, 0xc1, 0x8d, 0xfb, 0x77, 0xaf, 0x9e, 0xee, 0xa6,
0x2b, 0xe1, 0x8f, 0xb7, 0x1c, 0xe4, 0x60, 0x17, 0x96, 0xb3, 0x68, 0x2d, 0x6c, 0x7a, 0x8b, 0x84,
0x75, 0xe1, 0x20, 0x17, 0x05, 0x91, 0xd5, 0x32, 0xf3, 0xba, 0x87, 0x88, 0x87, 0xcc, 0xd6, 0xf7,
0x19, 0xf9, 0xe4, 0x39, 0x2a, 0xfe, 0x09, 0xa7, 0x76, 0xa2, 0x9b, 0xae, 0x98, 0x33, 0x42, 0x7f,
0x45, 0x10, 0x2c, 0xf9, 0xc1, 0x29, 0x68, 0xa9, 0x82, 0xc7, 0x3b, 0x09, 0x90, 0x27, 0x2d, 0x1e,
0x8b, 0x12, 0xf3, 0x98, 0x1d, 0x3d, 0x6c, 0xbe, 0x58, 0xa1, 0xc3, 0x03, 0xb5, 0x22, 0x0a, 0x57,
0x89, 0xbe, 0xea, 0x9e, 0xc3, 0x86, 0xc6, 0xf6, 0x31, 0x61, 0x90, 0x0c, 0x4e, 0xd6, 0xd7, 0x0d,
0x13, 0x33, 0x58, 0xfe, 0x33, 0xa4, 0x75, 0x62, 0xe3, 0x15, 0x70, 0xf2, 0xa1, 0x3a, 0x3f, 0x47,
0xc5, 0xe2, 0xc5, 0x94, 0x20, 0xbe, 0x7f, 0x21, 0x3d, 0x17, 0x3d, 0x2f, 0x37, 0xfe, 0xe7, 0xa0,
0x42, 0xfc, 0xaa, 0xbd, 0x92, 0xed, 0x00, 0x25, 0x12, 0xbe, 0xa6, 0x26, 0xf1, 0x06, 0x40, 0x6a,
0x0b, 0x9c, 0x47, 0xc9, 0xeb, 0xc6, 0x49, 0xd4, 0xea, 0xc5, 0x72, 0x61, 0xb4, 0x25, 0xb1, 0xe2,
0xe7, 0xe4, 0xe3, 0x71, 0x4f, 0x49, 0x15, 0x62, 0x3f, 0x39, 0x55, 0xe3, 0xf7, 0x28, 0x43, 0xab,
0x3d, 0xb4, 0x2b, 0x1a, 0x3a, 0xf7, 0x16, 0x3d, 0x0c, 0x2e, 0x21, 0xe6, 0xc2, 0x33, 0xa8, 0x80,
0xdb, 0x5a, 0x3f, 0xa5, 0x21, 0xa3, 0x0e, 0x3c, 0xcf, 0x1c, 0x5b, 0x5a, 0xac, 0x44, 0x59, 0xf7,
0xbf, 0x13, 0x52, 0x39, 0xcc, 0x7d, 0xe3, 0x01, 0x7c, 0x64, 0x53, 0x3a, 0x04, 0xdb, 0x7b, 0x09,
0xc8, 0xc1, 0x93, 0x83, 0x42, 0xda, 0xf6, 0x86, 0x45, 0x91, 0xa6, 0x94, 0xd5, 0xad, 0x76, 0x39,
0x76, 0xe2, 0x04, 0x11, 0x35, 0x1b, 0x81, 0xac, 0x33, 0x52, 0x63, 0xdd, 0xd6, 0x7d, 0xbb, 0xb8,
0x6d, 0xad, 0xa1, 0x9c, 0xa4, 0x15, 0x09, 0x24, 0xac, 0x94, 0xde, 0x1e, 0x6f, 0xf4, 0x13, 0x48,
0xd6, 0x71, 0xf6, 0x07, 0x0a, 0xce, 0x8a, 0xfd, 0x58, 0xa3, 0x8e, 0x01, 0x29, 0xa6, 0x9e, 0x58,
0xe3, 0xf2, 0x44, 0x41, 0xf6, 0xb7, 0x08, 0x8b, 0x8b, 0x16, 0x5d, 0xc5, 0x1d, 0x7a, 0x4f, 0xea,
0x25, 0x7c, 0x6e, 0x03, 0xb7, 0xdc, 0xc7, 0x8f, 0xbf, 0x83, 0x1e, 0x93, 0x92, 0x88, 0x26, 0xaa,
0xe6, 0x5e, 0xab, 0x2b, 0xff, 0x88, 0x66, 0xc2, 0x83, 0xde, 0x1a, 0x83, 0x3e, 0x69, 0x21, 0xdf,
0xb2, 0x9d, 0x3c, 0xf7, 0xf5, 0x56, 0xd0, 0x74, 0xe6, 0xf4, 0xdb, 0x51, 0x5d, 0x86, 0x5d, 0x28,
0xb9, 0xae, 0x2c, 0x37, 0x54, 0x66, 0x79, 0x8d, 0xd3, 0x0d, 0x4c, 0x79, 0xeb, 0x55, 0x6f, 0x27,
0x9b, 0xf5, 0xd7, 0x33, 0xb8, 0xd0, 0x22, 0x86, 0x2f, 0xde, 0xb1, 0x3a, 0x80, 0xd0, 0xc3, 0x28,
0x80, 0xfa, 0x8c, 0x96, 0xb9, 0x35, 0x6a, 0xd0, 0xde, 0x78, 0x45, 0xe0, 0xfc, 0x28, 0xc4, 0x03,
0x09, 0x94, 0x57, 0x52, 0x6f, 0xbf, 0x96, 0x88, 0xb8, 0xb8, 0x3d, 0xa9, 0x4c, 0x86, 0x8c, 0xaf,
0x22, 0x14, 0xd2, 0xbd, 0x3c, 0x45, 0xb1, 0x2e, 0x97, 0x27, 0xf4, 0xfb, 0xcd, 0x37, 0xe6, 0x16,
0x80, 0x4f, 0x42, 0xc6, 0x69, 0xe3, 0x9b, 0x30, 0xf9, 0xbf, 0xe1, 0x46, 0x3f, 0xed, 0xe7, 0x36,
0x7f, 0xdb, 0x41, 0xdd, 0x6e, 0x21, 0x58, 0x2b, 0x23, 0x79, 0x7d, 0x09, 0x44, 0x47, 0x29, 0xbd,
0x47, 0xb7, 0x22, 0xf7, 0xde, 0xe8, 0xb3, 0x76, 0x19, 0xf0, 0xbd, 0xe0, 0x21, 0x59, 0xc3, 0x45,
0xc1, 0x8f, 0xeb, 0x5e, 0xff, 0x45, 0x90, 0x61, 0x9b, 0x7d, 0x75, 0x2a, 0x66, 0x89, 0x3b, 0xd5,
0xc8, 0x62, 0x27, 0x01, 0x2f, 0xfc, 0x05, 0x06, 0x59, 0x67, 0x6a, 0x1d, 0xc0, 0xe5, 0x76, 0x88,
0x33, 0x7e, 0x7a, 0x1a, 0x86, 0xa7, 0x00, 0x29, 0xf4, 0xd7, 0x2b, 0x9f, 0xe1, 0x5e, 0xff, 0xa9,
0xf4, 0xe6, 0xc8, 0xb1, 0x7d, 0xd9, 0xfb, 0xed, 0xf6, 0xe0, 0x8c, 0x4b, 0x76, 0xf5, 0x06, 0x77,
0x43, 0x62, 0x5f, 0xd8, 0xe9, 0xad, 0xcd, 0x95, 0xa1, 0xb8, 0xa9, 0x95, 0x83, 0xda, 0x2f, 0x00,
0xd2, 0x01, 0x38, 0x86, 0x2d, 0x9e, 0xd3, 0x25, 0xe7, 0x88, 0xab, 0x75, 0xa5, 0xdb, 0x6c, 0x9d,
0xd6, 0x8d, 0xc5, 0x78, 0x84, 0x98, 0x94, 0x77, 0x7e, 0xb2, 0x0e, 0x0b, 0x25, 0xcc, 0xee, 0x7c,
0x4d, 0xd1, 0x0a, 0x7d, 0xc7, 0x98, 0x6d, 0x23, 0x56, 0xd4, 0x94, 0xd1, 0x28, 0x3c, 0x43, 0x1f,
0xf2, 0x4b, 0x1d, 0x71, 0x7b, 0xd8, 0xb8, 0x80, 0xdd, 0x7e, 0xb6, 0x4b, 0x55, 0x69, 0x20, 0xb4,
0xde, 0x16, 0x82, 0x4a, 0x6e, 0x5c, 0x79, 0xd1, 0xa0, 0x4d, 0x47, 0xc1, 0xa1, 0x10, 0x20, 0xc6,
0x25, 0xf3, 0x0d, 0xd8, 0xff, 0x19, 0x6c, 0xaa, 0x6c, 0x00, 0x6e, 0x31, 0x12, 0x30, 0x0d, 0x61,
0xe4, 0x23, 0xb7, 0x27, 0xa4, 0xfb, 0xa2, 0x80, 0xc6, 0x75, 0xa9, 0x04, 0x4c, 0xbd, 0x8f, 0xd0,
0x28, 0x93, 0x93, 0xe3, 0x4f, 0xa9, 0x33, 0x6f, 0x98, 0xe1, 0x59, 0xd7, 0x3e, 0xd1, 0x7e, 0xf0,
0x3f, 0xc3, 0xb8, 0x16, 0xcb, 0xe9, 0xd8, 0x21, 0x65, 0x18, 0x0e, 0xf1, 0x72, 0x16, 0xda, 0x60,
0x77, 0xfd, 0x39, 0x67, 0xf6, 0x22, 0x27, 0xa7, 0x3c, 0x37, 0x81, 0x2b, 0x25, 0xa5, 0xec, 0x1b,
0xf2, 0x98, 0x59, 0x75, 0xe5, 0xb9, 0x76, 0x06, 0xc1, 0xf3, 0xe9, 0x33, 0x14, 0x5e, 0x02, 0x5b,
0xe6, 0xdb, 0x7b, 0xa6, 0xff, 0x5b, 0xfc, 0xf4, 0xc6, 0xec, 0x99, 0xb0, 0x95, 0x71, 0xcc, 0xc4,
0x8d, 0xc2, 0xa5, 0xca, 0x34, 0xa6, 0x17, 0x6b, 0x3a, 0x9c, 0x50, 0x4c, 0x5d, 0x3e, 0x7b, 0xdb,
0x31, 0x75, 0x0b, 0x67, 0x87, 0x06, 0xdb, 0x42, 0x65, 0xaf, 0x40, 0x4b, 0xa6, 0x1d, 0xa5, 0x08,
0x42, 0xc0, 0xc0, 0xac, 0xa9, 0x86, 0x70, 0x3d, 0xb8, 0x90, 0xe2, 0x19, 0xba, 0xa9, 0x53, 0xdc,
0x46, 0xaa, 0x25, 0x52, 0x86, 0x43, 0xf5, 0xa2, 0x48, 0x5e, 0xd8, 0x35, 0xb7, 0x0c, 0x37, 0x8d,
0x24, 0xe4, 0xd4, 0x38, 0xa3, 0x82, 0x57, 0x93, 0x2f, 0x74, 0x88, 0xce, 0x3b, 0x18, 0x9c, 0x68,
0x57, 0x97, 0x94, 0x02, 0xba, 0x90, 0x0f, 0xc8, 0x50, 0x61, 0x0a, 0x8e, 0x46, 0x80, 0x45, 0xb3,
0x21, 0xc6, 0x51, 0xf6, 0xfe, 0xd8, 0x86, 0x90, 0x94, 0x19, 0x50, 0x47, 0x17, 0x35, 0xdc, 0xaa,
0x0c, 0x22, 0x6d, 0xbb, 0x6c, 0x5a, 0xd1, 0x4b, 0x97, 0xb4, 0x5b, 0x7d, 0x44, 0x38, 0x22, 0x70,
0x8c, 0x92, 0x30, 0xd8, 0xbf, 0xb4, 0x72, 0xcf, 0xf3, 0xdd, 0xa2, 0xc5, 0x94, 0x88, 0x24, 0x0e,
0x58, 0x8a, 0x02, 0x14, 0x6a, 0x56, 0x93, 0xa2, 0xf5, 0x7c, 0x14, 0x48, 0x24, 0xa6, 0x49, 0x7b,
0x9d, 0xea, 0xa7, 0x5f, 0xfa, 0x24, 0xa2, 0x3c, 0xbf, 0xbd, 0x0e, 0x0a, 0x3c, 0x15, 0x36, 0xbc,
0x4b, 0xf8, 0x65, 0xf7, 0x74, 0x50, 0x55, 0xd9, 0x9d, 0x6e, 0x04, 0x0f, 0xe4, 0x83, 0x12, 0x61,
0x9a, 0x22, 0x18, 0xd1, 0xae, 0xca, 0xa4, 0xbc, 0xfa, 0x82, 0x9c, 0xa1, 0xcc, 0x19, 0x55, 0xe8,
0x93, 0x9b, 0x8b, 0xe0, 0x54, 0x95, 0x9e, 0xec, 0x12, 0x5c, 0x61, 0xe3, 0xf9, 0x9a, 0x17, 0x0d,
0x2e, 0xa0, 0x0a, 0x5b, 0x69, 0xa0, 0xce, 0xb2, 0xa2, 0x9b, 0x0a, 0x29, 0x14, 0x1c, 0x6a, 0x0e,
0xcc, 0x4e, 0x71, 0x08, 0xd6, 0x95, 0x42, 0xf1, 0xac, 0x48, 0x92, 0xcd, 0xb9, 0x9d, 0xdb, 0x6e,
0xf8, 0xd0, 0xda, 0xbe, 0x1b, 0x2b, 0x37, 0x18, 0x82, 0xe6, 0x91, 0x6e, 0xdf, 0xde, 0xe2, 0x6c,
0x29, 0x32, 0x74, 0x5f, 0xc8, 0x83, 0x42, 0xfe, 0x72, 0x9d, 0x20, 0x99, 0x71, 0x9e, 0xa6, 0xaf,
0x8b, 0x63, 0x2a, 0xbf, 0x67, 0x33, 0x91, 0xdc, 0x03, 0x5b, 0x67, 0x78, 0x2a, 0x4c, 0x31, 0x60,
0x73, 0x67, 0x21, 0x16, 0xc4, 0x7b, 0x7d, 0x19, 0x22, 0xbb, 0xb7, 0xad, 0xe3, 0x1c, 0x10, 0x79,
0xcc, 0x1d, 0xc3, 0x95, 0xc2, 0x6a, 0xe1, 0x25, 0xe1, 0xf3, 0xb3, 0xe0, 0x3c, 0xae, 0xbe, 0x65,
0xbc, 0x0d, 0x73, 0x68, 0x20, 0x56, 0x43, 0x25, 0x77, 0xc0, 0x0a, 0xc9, 0x1a, 0xb1, 0x66, 0xe1,
0x06, 0xd0, 0x41, 0x51, 0x4d, 0xbc, 0xf0, 0xc9, 0xaa, 0xed, 0x6a, 0xc6, 0xdd, 0xd8, 0x83, 0xdf,
0xca, 0x99, 0x62, 0x32, 0xe4, 0x91, 0xba, 0x69, 0xdb, 0xff, 0xdc, 0x11, 0x02, 0x50, 0x15, 0x6f,
0x95, 0xf5, 0xc7, 0x55, 0x67, 0xd3, 0xc9, 0x36, 0x65, 0xa7, 0x97, 0x51, 0xde, 0x1a, 0x10, 0x9d,
0x92, 0xce, 0x4c, 0x46, 0x79, 0xf9, 0xc4, 0xcb, 0x91, 0xe0, 0xe1, 0x30, 0xfe, 0x27, 0xf4, 0xf0,
0x03, 0x47, 0x2b, 0x52, 0x9c, 0x3d, 0x3b, 0x47, 0x5d, 0x6a, 0x5b, 0x6b, 0x28, 0x18, 0x31, 0xa4,
0x66, 0x84, 0x4e, 0x1f, 0xf2, 0x10, 0x6c, 0x0a, 0x5e, 0x77, 0x52, 0xbe, 0x61, 0x06, 0xfa, 0x22,
0x7c, 0xc9, 0x94, 0xcb, 0xdf, 0xef, 0x6a, 0xdf, 0xca, 0x17, 0x3a, 0x7c, 0xe7, 0xaa, 0x5a, 0x3c,
0x04, 0x6d, 0xb7, 0x07, 0x66, 0x07, 0xcd, 0x33, 0xec, 0x65, 0x1a, 0xb5, 0xf3, 0x05, 0xda, 0x89,
0x82, 0xa1, 0x78, 0x42, 0xe5, 0x6e, 0xd7, 0xa5, 0xb5, 0x2e, 0x5c, 0xb6, 0x9b, 0x9f, 0xf9, 0x96,
0xe6, 0x50, 0xec, 0x02, 0x65, 0xc8, 0x59, 0x2d, 0x7a, 0x00, 0xa4, 0x6e, 0x15, 0x71, 0x0f, 0x6e,
0x45, 0xbc, 0x8f, 0x54, 0xa1, 0xa8, 0xe9, 0x97, 0x38, 0x9f, 0xf7, 0xe2, 0xed, 0x85, 0x5b, 0x48,
0x4c, 0x3c, 0xe6, 0xea, 0xf8, 0x4b, 0x3c, 0x98, 0x6b, 0x4d, 0xf5, 0x2b, 0x71, 0xfe, 0x5e, 0x55,
0x6b, 0xb6, 0xe2, 0xeb, 0x3c, 0x5b, 0x6b, 0x79, 0x31, 0x71, 0xad, 0x66, 0x0c, 0x02, 0xf3, 0x9c,
0x12, 0x0c, 0x68, 0xae, 0x6c, 0x43, 0x92, 0xb0, 0xc0, 0x28, 0xed, 0x62, 0x9d, 0x5a, 0xf8, 0x5b,
0x22, 0xcd, 0xe5, 0x13, 0x42, 0xf0, 0x4e, 0x33, 0xab, 0x40, 0x90, 0x1d, 0x2c, 0xb0, 0x7a, 0xf0,
0x1e, 0xfd, 0xb5, 0xd7, 0xdf, 0x13, 0x5c, 0x29, 0xe2, 0xef, 0x06, 0x6e, 0xcf, 0xf5, 0xea, 0xa1,
0xa0, 0x06, 0x5f, 0xa1, 0x33, 0x23, 0x23, 0x06, 0x2e, 0xdc, 0x19, 0x5d, 0xca, 0x2d, 0xb8, 0x5f,
0x2d, 0x0f, 0x70, 0x7a, 0x91, 0x18, 0xc0, 0x51, 0x52, 0x3a, 0xa2, 0xd6, 0xa0, 0xf9, 0x18, 0x30,
0xee, 0x93, 0x25, 0x3e, 0x59, 0xec, 0xe3, 0xed, 0x21, 0x0a, 0xb8, 0xb9, 0xeb, 0xb7, 0x6d, 0xc4,
0x40, 0x76, 0x4b, 0xeb, 0xbe, 0xac, 0xd1, 0x1f, 0x6d, 0x7a, 0x6a, 0x8e, 0x1b, 0x6b, 0xd0, 0xf9,
0x97, 0x5a, 0xcd, 0xef, 0xf9, 0x70, 0xd3, 0x90, 0x17, 0xf4, 0x41, 0x6e, 0x42, 0x9d, 0x5c, 0xfa,
0x6d, 0xf5, 0x2e, 0xe0, 0xca, 0x87, 0x2b, 0xb0, 0x2f, 0x35, 0x72, 0x44, 0x2e, 0x79, 0x91, 0x6e};


static s32 globalRandomNext = 0; // This must always be < ArrayCount(globalRandomTable)

//
// Random full value range
//

inline u8 RandomU8(){
	s32 count = ArrayCount(globalRandomTable);
	AssertRange(0, globalRandomNext, ArrayCount(globalRandomTable) - 1);
	u8 result = globalRandomTable[globalRandomNext];
	globalRandomNext = (globalRandomNext + 1) % ArrayCount(globalRandomTable);
	return result;
}
inline u16 RandomU16(){
	AssertRange(0, globalRandomNext, ArrayCount(globalRandomTable) - 1);
	if (globalRandomNext + 2 >= ArrayCount(globalRandomTable)){
		globalRandomNext -= ArrayCount(globalRandomTable) - 2;
	}
	u16 result = *(u16 *)(globalRandomTable + globalRandomNext);
	globalRandomNext += 2;
	return result;
}
inline u32 RandomU32(){
	AssertRange(0, globalRandomNext, ArrayCount(globalRandomTable) - 1);
	if (globalRandomNext + 4 >= ArrayCount(globalRandomTable)){
		globalRandomNext -= ArrayCount(globalRandomTable) - 4;
	}
	u32 result = *(u32 *)(globalRandomTable + globalRandomNext);
	globalRandomNext += 4;
	return result;
}
inline s32 RandomS32(){
	s32 result = (s32)RandomU32();
	return result;
}

// 0 and 1 are inclusive
inline f32 Random01(){
	f32 result = (f32)RandomU32() / 4294967295.f; // magic number is 2^32 -1
	return result;
}


//
// Random in a specific range
//

// Return range: [min, max] (both inclusive)
inline f32 RandomRange(f32 min, f32 max){
	f32 t = Random01();
	f32 result = (1.f - t)*min + max*t;
	return result;
}
// Return range: ~[min, max]
// WARNING: The result could actually be outside [min, max] when min or max are huge (>2^24) because of precision issues.
inline s32 RandomRangeS32(s32 min, s32 max){
	s32 result = (s32)RandomRange((f32)min, (f32)max);
	return result;
}
// Return range: ~[min, max]
// WARNING: The result could actually be outside [min, max] when min or max are huge (>2^24) because of precision issues.
inline u32 RandomRangeU32(u32 min, u32 max){
	u32 result = (u32)RandomRange((f32)min, (f32)max);
	return result;
}
// Return range: [min, max]
inline u16 RandomRangeU16(u16 min, u16 max){
	u32 randomBits = RandomU32();
	u16 result = min + (u16)(randomBits % ((u32)max - (u32)min + 1));
	return result;
}
// Return range: [min, max]
inline u8 RandomRangeU8(u8 min, u8 max){
	u32 randomBits = RandomU32();
	u8 result = min + (u8)(randomBits % ((u32)max - (u32)min + 1));
	return result;
}


//
// Random [0, max]
//

// Random number in range [0, max] (both inclusive)
inline f32 Random(f32 max){
	return RandomRange(0, max);
}
// Return range: [0, max]
inline s32 RandomS32(s32 max){
	s32 result = (s32)RandomRange(0.f, (f32)max);
	return result;
}
// Return range: [0, max]
inline u32 RandomU32(u32 max){
	u32 result = (u32)RandomRange(0.f, (f32)max);
	return result;
}
// Return range: [0, max]
inline u16 RandomU16(u16 max){
	u32 randomBits = RandomU32();
	u16 result = (u16)(randomBits % ((u32)max + 1));
	return result;
}
// Return range: [0, max]
inline u8 RandomU8(u8 max){
	u32 randomBits = RandomU32();
	u8 result = (u8)(randomBits % ((u32)max + 1));
	return result;
}


// If chance <=0 the result will always be false.
// If chance >=1 the result will always be true.
inline b32 RandomChance(f32 chance){
	// 16777216 is 2^24, and is the last consecutive integer perfectly representable by a float.
	u32 randomBits = RandomU32() & 16777215; // 24 random bits.
	f32 randomNumber = (f32)randomBits; // Range: [0, 16777215]
	f32 threshold = chance*16777216.f;  // Range: [0, 16777216]
	if (randomNumber < threshold)
		return true;
	return false;
}


u32 Hash(u32 x) {
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    return x;
}

void ShuffleRandomState(u32 seed){
	u32 h = Hash(seed ^ RandomU32());
	globalRandomNext = h % ArrayCount(globalRandomTable);
}

#endif