
{
    "targets": [
		{
        "target_name": "nimg",
        "sources": [
            "nimg.cpp",
        ],
        'include_dirs': [
			"<!@(node -p \"require('node-addon-api').include\")",
            'lib',
        ],
        'conditions': [
            ['OS=="mac"', {
                'xcode_settings': {
                    'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
                    'OTHER_CPLUSPLUSFLAGS' : [ 
						'-mmacosx-version-min=10.7',
                        '-stdlib=libc++',
                        '-std=c++17'
					]
                },
                'include_dirs': ['/usr/include/malloc']
            }],
        ]
    }]
}

