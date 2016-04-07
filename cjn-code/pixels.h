

struct Pixel {
	int addr;
	uint32 color;
	struct Pixel *next;
};

struct Comet {
	struct Node *target_node;
	int next_addr;
	int path_len;
	int incr;
	
	struct Comet *next_comet;
	struct Pixel *head_px;
}

void moveComet(Pixel *px);
void cometNextAddr(Comet *comet);
void handleNode(Comet *comet);