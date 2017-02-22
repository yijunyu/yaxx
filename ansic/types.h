#define TYPEDEF_C 1
struct type_info_t {
	int storage_class;
	int type;
	int qualifier;
	int function: 1;
	int array: 1;
};

