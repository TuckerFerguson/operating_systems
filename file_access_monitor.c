struct file_access_monitor {
	int max;
	int total;
	pthread_cond_t ok_to_access;
	pthread_mutex_t lock;
}

void initialize_monitor(int max_number_of_processes, struct *file_access_monitor monitor)
{
	monitor = (struct *file_access_monitor)malloc(sizeof(struct file_access_monitor));
	monitor->max = max_number_of_processes;
	monitor->total = 0;
	pthread_cond_init(&monitor->ok_to_access, NULL);	
	pthread_mutex_init(&monitor->lock, NULL);
}

void access_file(struct *file_access_monitor monitor)
{
	pthread_mutex_lock(&monitor->lock);
	while (total+1 > max) 
		pthread_condition_wait(&monitor->ok_to_access);
	total++;
	pthread_mutex_unlock(&monitor->lock); 
}

void release_file(struct *file_access_monitor monitor)
{
	pthread_mutex_lock(&monitor->lock);
	pthread_condition_signal(&monitor->ok_to_access);
	total--;
	pthread_mutex_unlock(&monitor->lock); 	
}