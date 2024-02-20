#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/queue.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

typedef uint32_t u32;
typedef int32_t i32;

struct process
{
  u32 pid;
  u32 arrival_time;
  u32 burst_time;

  TAILQ_ENTRY(process) pointers;

  /* Additional fields here */
  u32 start_time;
  u32 remaining_time;
  bool is_start_time_set;
  /* End of "Additional fields here" */
};

TAILQ_HEAD(process_list, process);

u32 next_int(const char **data, const char *data_end)
{
  u32 current = 0;
  bool started = false;
  while (*data != data_end)
  {
    char c = **data;

    if (c < 0x30 || c > 0x39)
    {
      if (started)
      {
        return current;
      }
    }
    else
    {
      if (!started)
      {
        current = (c - 0x30);
        started = true;
      }
      else
      {
        current *= 10;
        current += (c - 0x30);
      }
    }

    ++(*data);
  }

  printf("Reached end of file while looking for another integer\n");
  exit(EINVAL);
}

u32 next_int_from_c_str(const char *data)
{
  char c;
  u32 i = 0;
  u32 current = 0;
  bool started = false;
  while ((c = data[i++]))
  {
    if (c < 0x30 || c > 0x39)
    {
      exit(EINVAL);
    }
    if (!started)
    {
      current = (c - 0x30);
      started = true;
    }
    else
    {
      current *= 10;
      current += (c - 0x30);
    }
  }
  return current;
}

void init_processes(const char *path,
                    struct process **process_data,
                    u32 *process_size)
{
  int fd = open(path, O_RDONLY);
  if (fd == -1)
  {
    int err = errno;
    perror("open");
    exit(err);
  }

  struct stat st;
  if (fstat(fd, &st) == -1)
  {
    int err = errno;
    perror("stat");
    exit(err);
  }

  u32 size = st.st_size;
  const char *data_start = mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);
  if (data_start == MAP_FAILED)
  {
    int err = errno;
    perror("mmap");
    exit(err);
  }

  const char *data_end = data_start + size;
  const char *data = data_start;

  *process_size = next_int(&data, data_end);

  *process_data = calloc(sizeof(struct process), *process_size);
  if (*process_data == NULL)
  {
    int err = errno;
    perror("calloc");
    exit(err);
  }

  for (u32 i = 0; i < *process_size; ++i)
  {
    (*process_data)[i].pid = next_int(&data, data_end);
    (*process_data)[i].arrival_time = next_int(&data, data_end);
    (*process_data)[i].burst_time = next_int(&data, data_end);
  }

  munmap((void *)data, size);
  close(fd);
}

int main(int argc, char *argv[])
{
  if (argc != 3)
  {
    return EINVAL;
  }
  struct process *data;
  u32 size;
  init_processes(argv[1], &data, &size);

  u32 quantum_length = next_int_from_c_str(argv[2]);

  struct process_list list;
  TAILQ_INIT(&list);

  u32 total_waiting_time = 0;
  u32 total_response_time = 0;

  /* Your code here */

// Initializing the queue and sorting it based on arrival time and PID
for (int i = 0; i < size; i++)
{
  struct process* new_proc = (struct process*)malloc(sizeof(struct process));
  new_proc->pid = data[i].pid;
  new_proc->arrival_time = data[i].arrival_time;
  new_proc->burst_time = data[i].burst_time;
  new_proc->is_start_time_set = false;
  new_proc->remaining_time = data[i].burst_time;
  TAILQ_INSERT_TAIL(&list, new_proc, pointers);
}

u32 current_time = 0;
struct process_list ready_queue;
TAILQ_INIT(&ready_queue);
struct process* previous_task = NULL;

// Main scheduling loop
while(!(TAILQ_EMPTY(&ready_queue) && TAILQ_EMPTY(&list) && previous_task == NULL))
{
  // Adding arriving tasks to the ready queue
  if(!TAILQ_EMPTY(&list))
  {
    struct process* proc;
    while(!TAILQ_EMPTY(&list))
    {
      proc = TAILQ_FIRST(&list);
      if(proc->arrival_time <= current_time)
      {
        TAILQ_REMOVE(&list, proc, pointers);
        TAILQ_INSERT_TAIL(&ready_queue, proc, pointers);
      }
      else
        break;
    }
  }

  if(previous_task != NULL)
    TAILQ_INSERT_TAIL(&ready_queue, previous_task, pointers);

  // Executing tasks in the ready queue
  if(!TAILQ_EMPTY(&ready_queue))
  {
    struct process* current_task = TAILQ_FIRST(&ready_queue);
    TAILQ_REMOVE(&ready_queue, current_task, pointers);

    if(!current_task->is_start_time_set)
    {
      current_task->start_time = current_time;
      current_task->is_start_time_set = true;
      total_response_time += current_time - current_task->arrival_time;
    }

    if(current_task->remaining_time > quantum_length)
    {
      current_task->remaining_time -= quantum_length;
      previous_task = current_task;
    }
    else
    {
      total_waiting_time += ((current_time + current_task->remaining_time) - current_task->arrival_time - current_task->burst_time);
      if(current_task->remaining_time < quantum_length)
         current_time -= (quantum_length - current_task->remaining_time);
      free(current_task);
      previous_task = NULL;
    }
  }
  current_time += quantum_length;
}

  /* End of "Your code here" */

  printf("Average waiting time: %.2f\n", (float)total_waiting_time / (float)size);
  printf("Average response time: %.2f\n", (float)total_response_time / (float)size);

  free(data);
  return 0;
}
