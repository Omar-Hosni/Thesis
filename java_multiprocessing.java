public static void main(String[] args) {
    int N = 10000000;
    final long[] result = {0};
    Lock lock = new ReentrantLock();
    
    int numProcesses = 4;
    ExecutorService executor = Executors.newFixedThreadPool(numProcesses);
    
    for (int i = 0; i < numProcesses; i++) {
        final int start = 1 + (i * (N / numProcesses));
        final int end = (i + 1) * (N / numProcesses);
        
        Future<?> future = executor.submit(() -> {
            long partialSum = 0;
            for (int j = start; j < end; j++) {
                partialSum += j;
            }
            
            lock.lock();
            try {
                result[0] += partialSum;
            } finally {
                lock.unlock();
            }
        });
    }
    
    executor.shutdown();
}
