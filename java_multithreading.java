public static void main(String[] args) {
    int N = 10000000;
    AtomicInteger result = new AtomicInteger();
    Object lock = new Object();
    
    int numThreads = 4;
    Thread[] threads = new Thread[numThreads];
    
    for (int i = 0; i < numThreads; i++) {
        int finalI = i;
        threads[i] = new Thread(() -> {
            int partialSum = 0;
            for (int j = 1 + finalI; j <= N; j += numThreads) {
                partialSum += j;
            }
            
            synchronized (lock) {
                result.addAndGet(partialSum);
            }
        });
        threads[i].start();
    }
    
    for (Thread thread : threads) {
        try {
            thread.join();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}
