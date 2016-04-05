// PATH METADATA
#define PATH_END		0x0001		// Which end of the path the node is at.
//#define 		0x0002
//#define 		0x0004
//#define 		0x0008
//#define 		0x0010
//#define 		0x0020
//#define 		0x0040
//#define 		0x0080


// Represents where 3 or more paths intersect. May have a pixel in the center.
typedef struct node{
	// Address of the center pixel
	int ctr_px_addr;
	
	// Number of branches from this node
	int num_branches;
	
	// Array of possible paths
	struct PathMeta branches[MAX_BRANCHES];
} node_t;

// Associates a specific node to a specific path, mainly so the node knows which end of the path it is starting at.
typedef struct path_meta {
	// Information about the path
	// IMPORTANT! The "PATH_END" flag must indicate the array index of the OTHER node in the path struct, while indicating the array index of the pixel address of the CLOSEST pixel on the path.
	int path_info;
	
	// Pointer to the path itself
	struct path_t *path;
} path_meta_t;

// Describes a chain of pixels from one node to another
typedef struct path {
	// How many pixels in the path
	int len;
	
	// The end pixel addresses
	int end_px_addr[2];
	
	// The end nodes
	struct node_t *end_nodes[2];
} path_t;