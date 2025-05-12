public class TaskManager {

    private final TaskQueue queue;

    public TaskManager(ThreadGroup group, int threadCount) {
        queue = new TaskQueue();

        for (int i = 0; i < threadCount; i++) {
            Thread thread = new Thread(group, new TaskHandling(queue));
            thread.start();
        }
    }

    public synchronized void doTask(Runnable task) {
        queue.addTask(task);
    }
}

class TaskHandling implements Runnable {
    private final TaskQueue taskQueue;

    TaskHandling(TaskQueue taskQueue) {
        this.taskQueue = taskQueue;
    }

    @Override
    public void run() {
        while (true) {
            Runnable task = taskQueue.getTask();
            if (task != null) {
                task.run();
            }
        }
    }
}

class TaskQueue {

private final Runnable[] tasks = new Runnable[100]; 
private int head = 0;
private int tail = 0;

public synchronized void addTask(Runnable task) {
    while ((tail + 1) % tasks.length == head) { 
        try {
            wait(); 
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
    }
    tasks[tail] = task;
    tail = (tail + 1) % tasks.length;
    notifyAll(); 
}

public synchronized Runnable getTask() {
    while (head == tail) { 
        try {
            wait(); 
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
    }
    Runnable task = tasks[head];
    head = (head + 1) % tasks.length;
    notifyAll(); 
    return task;
}
}
