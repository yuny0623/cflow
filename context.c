/* ## Context ## 
 * manage small chunk of data which is shared across tasks execution 
 */

// shoule we use file? or just a memory ?


#include<stdio.h>
#include<stdbool.h>
#include<memory.h>
#include<time.h> // <-- for ctx creation time 
#include<string.h>

#define INITIAL_CTX_ENT_CAPACITY 100;  // initial size is set to 100, how can we expand this capacity... need more consideration 

typedef enum { 
	CTX_MEMORY_MODE = 1; 
	CTX_FILE_MODE = 1; 
} CtxMode; 

struct Entry {
	const char* key; 	// key
	char* str_val; 
	int int_val; 
	double double_val; 
	char* memo;       	// memo 
}

struct Context {  
	int mode; // CTX_MEMORY_MODE, CTX_FILE_MODE, 
	int capacity; 
	int fd;   // File descriptor 
	double created_at; 
	double updated_at; 

	// array of CtxEntry pointer 
	CtxEntry* entries[]; // it is good to locate VLA in the end of struct  
}; 

Context* create_ctx(int mode) { 
	Context* ctx; 

	switch (mode) {
		case CTX_MEMORY_MODE: 
			ctx = create_mem_ctx();
		      	break;  	
		case CTX_FILE_MODE: 
			ctx = create_fs_ctx();
		       	break; 	
		default: 
			ctx = create_mem_ctx(); // use memory mode by default; 
			break; 
	}

	fprintf(stdout, "create context with mode: %d\n", mode); 
	return ctx; 
}

static int64_t get_creation_time() { 
	struct timespec ts; 
	clock_gettime(CLOCK_REALTIME, &ts); 
	int64_t epoch_millsec = (int64_t) ts.tv_sec * 1000 + (int64_t) ts.tv_usec * 1000000;
       	return epoch_millsec; 	
}


static void init_ctx(Context* ctx) {
	fprintf(stdout, "init ctx\n"); 
	int64_t curr_epoch_millsec = get_creation_time(); 
	ctx->capacity = INITIAL_CTX_ENT_CAPACITY; 
	ctx->created_at = curr_epoch_millsec; 
	ctx->updated_at = curr_epoch_millsec; 
}

// create ctx using memory, returns pointer of Context
static Context* create_mem_ctx() { 
	fprintf(stdout, "create mem ctx\n"); 
	Context * ctx = calloc(1, sizeof(Context) + sizeof(Entry) * INITIAL_CTX_ENT_CAPACITY);
	init_ctx(ctx);  // initialize ctx 

	if (ctx == NULL) {
		perror("calloc"); 
		fprintf(stderr, "Error occured - create_mem_ctx\n"); 
		return NULL; 
	}
	return ctx; 
}

// create ctx using file system, returns file descriptor  
static int create_fs_ctx() { 
	fprintf(stdout, "create fs ctx\n"); 
	int fd = open("CFLOW_FS_", O_CREAT | O_TRUNC | O_RDWR, 0644); 
	if (fd == -1) {
		perror("open"); 
		return NULL; 
	}

	fprintf(stdout, "create fx ctx: %d\n", fd); 
	return fd; 
}

int ctx_ent_put(Context* ctx, Entry ent, const char* key) { 
	Entry * new_ent = calloc(1, sizeof(Entry));
	if (ent == NULL) {
		perror("calloc"); 
		return -1;
	}

	new_ent.key = ent.key;
	new_ent.str_val = ent.str_val; 
	new_ent.int_val = ent.int_val; 
	new_ent.double_val = ent.double_val; 
	new_ent.memo = ent.memo; 

	ctx->entries[ctx->capacity] = new_ent; 

	fprintf(stdout, "ctx ent put: %s\n", key); 
	return 0; 
}

void ctx_ent_del(Context* ctx, const char* key) { 
	Entry* ent = ctx_get(ctx, key); 
	free(ent); 
	fprintf(stdout, "ctx ent del: %s\n", key); 
}


// we need to improve this get mechanism, what about key hashing ? 
Entry* ctx_get(Context* ctx, const char* key, int capacity) { 	
	Entry* entries = ctx->entries; 
	for(int i = 0; i < capacity, i++) {
		if(entries[i] == NULL) {
			continue; 
		}

		if (strcmp(entries[i].key, key) == 0) {
			return entries[i]; 
		}
	}
	
	return NULL; 
}

int delete_context(Context* ctx) {
	int retry = 0;
	if (ctx->mode == CTX_MEMORY_MODE) {
		free(ctx); 
	} else {
		retry: 
		if(close(ctx->fd) == -1){
			perror("close"); 
			if (errno == EINTR) {
				retry += 1; 
				if (retry == 3) {
					fprintf(stderr, "non recoverable problem occured with fd: %d\n", fd); 
					free(ctx); 
					return -1; 
				}
				goto retry; 
			}
		} 
		free(ctx); 
	}
	fprintf(stdout, "delete context\n"); 
	return 0; 
}
