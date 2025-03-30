package cp2024.solution;

import cp2024.circuit.CircuitNode;
import cp2024.circuit.LeafNode;
import cp2024.circuit.NodeType;
import cp2024.circuit.ThresholdNode;

import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.RecursiveTask;

public class RecursiveSolver extends RecursiveTask<Boolean> {
    private final CircuitNode current;
    private final BlockingQueue<Boolean> storeResult;
    private final ParallelCircuitSolver solver;
    private boolean goodCancel = false; // checks if we were canceled by another node
    private boolean ifOK = true; // checks if all went according to plan (i.e. none external interrupts)

    public RecursiveSolver( CircuitNode c, BlockingQueue<Boolean> q, ParallelCircuitSolver s ) {
        current = c;
        storeResult = q;
        solver = s;
    }

    public boolean wentOK() {
        return ifOK;
    }

    private void setGoodCancel() {
        goodCancel = true;
    }

    private boolean shouldCancel() {
        return this.isCancelled() || solver.is_stopped();
    }

    public Boolean compute() {
        try {
            return recursiveSolve(current, storeResult);
        } catch ( InterruptedException e ) {
            return false;
        }
    }

    private boolean recursiveSolve( CircuitNode c, BlockingQueue<Boolean> q ) throws InterruptedException {
        if ( shouldCancel() ) {
            if ( !goodCancel )
                throw new InterruptedException();

            return false;
        }

        RecursiveSolver[] smaller_tasks = new RecursiveSolver[0];

        try {
            boolean res;
            CircuitNode[] args = c.getArgs();
            smaller_tasks = new RecursiveSolver[args.length];

            if ( c.getType() == NodeType.LEAF ) {
                res = ((LeafNode) c).getValue();

                if ( q != null )
                    q.add(res);

                return res;
            }

            res = switch ( c.getType() ) {
                case IF -> solveIF(args);
                case AND -> solveAND( args, smaller_tasks );
                case OR -> solveOR( args, smaller_tasks );
                case GT -> solveGT( args, ((ThresholdNode) c).getThreshold(), smaller_tasks );
                case LT -> solveLT( args, ((ThresholdNode) c).getThreshold(), smaller_tasks );
                case NOT -> solveNOT(args);
                default -> throw new RuntimeException();
            };

            if ( shouldCancel() ) {
                if ( !goodCancel )
                    throw new InterruptedException();

                return false;
            }

            if ( q != null )
                q.add(res);

            return res;

        } catch ( InterruptedException e ) {
            if ( goodCancel )
                for ( RecursiveSolver r : smaller_tasks )
                    r.setGoodCancel();

            for ( RecursiveSolver r : smaller_tasks )
                r.cancel(true);

            if ( !goodCancel ) {
                ifOK = false;
                throw new InterruptedException();
            }

            return false;
        }
    }

    private boolean solveNOT( CircuitNode[] args ) throws InterruptedException {
        return  !recursiveSolve( args[0], null );
    }

    private boolean solveLT( CircuitNode[] args, int threshold, RecursiveSolver[] smaller_tasks ) throws InterruptedException {
        int gotTrue = 0;

        if ( args.length == 1 ) {
            return !recursiveSolve(args[0], null);
        }

        BlockingQueue<Boolean> results = new LinkedBlockingQueue<>();

        for ( int i = 0; i < smaller_tasks.length; i++ ) {
            if ( shouldCancel() ) {
                if ( !goodCancel )
                    throw new InterruptedException();

                return false;
            }

            smaller_tasks[i] = new RecursiveSolver( args[i], results, solver );
        }

        for ( RecursiveSolver r : smaller_tasks ) {
            if ( shouldCancel() ) break; // We need to cancel RecursiveSolvers we have made.

            r.fork();
        }

        for ( int i = 0; i < smaller_tasks.length; i++ ) {
            if ( shouldCancel() ) {
                if ( !goodCancel )
                    throw new InterruptedException();

                break; // We need to cancel RecursiveSolvers we have made.
            }

            gotTrue += results.take() ? 1 : 0;
            if ( (gotTrue >= threshold) || (threshold - gotTrue > smaller_tasks.length - i - 1) )
                break; // We need to cancel RecursiveSolvers we have made.
        }

        for ( RecursiveSolver r : smaller_tasks )
            r.setGoodCancel();

        for ( RecursiveSolver r : smaller_tasks )
            r.cancel(true);

        return gotTrue < threshold;
    }

    private boolean solveGT( CircuitNode[] args, int threshold, RecursiveSolver[] smaller_tasks ) throws InterruptedException {
        int gotTrue = 0;
        BlockingQueue<Boolean> results = new LinkedBlockingQueue<>();

        for ( int i = 0; i < smaller_tasks.length; i++ ) {
            if ( shouldCancel() ) {
                if ( !goodCancel )
                    throw new InterruptedException();

                return false;
            }

            smaller_tasks[i] = new RecursiveSolver( args[i], results, solver );
        }

        for ( RecursiveSolver r : smaller_tasks ) {
            if ( shouldCancel() ) break; // We need to cancel RecursiveSolvers we have made.

            r.fork();
        }

        for ( int i = 0; i < smaller_tasks.length; i++ ) {
            if ( shouldCancel() ) {
                if ( !goodCancel )
                    throw new InterruptedException();

                break; // We need to cancel RecursiveSolvers we have made.
            }

            gotTrue += results.take() ? 1 : 0;
            if ( (gotTrue > threshold) || (threshold - gotTrue >= smaller_tasks.length - i - 1) )
                break; // We need to cancel RecursiveSolvers we have made.
        }

        for ( RecursiveSolver r : smaller_tasks )
            r.setGoodCancel();

        for ( RecursiveSolver r : smaller_tasks )
            r.cancel(true);

        return gotTrue > threshold;
    }

    private boolean solveOR( CircuitNode[] args, RecursiveSolver[] smaller_tasks ) throws InterruptedException {
        BlockingQueue<Boolean> results = new LinkedBlockingQueue<>();

        for ( int i = 0; i < smaller_tasks.length; i++ ) {
            if ( shouldCancel() ) {
                if ( !goodCancel )
                    throw new InterruptedException();

                return false;
            }

            smaller_tasks[i] = new RecursiveSolver( args[i], results, solver );
        }

        for ( RecursiveSolver r : smaller_tasks ) {
            if ( shouldCancel() ) break;

            r.fork();
        }

        boolean res = false;
        for ( int i = 0; i < smaller_tasks.length; i++ ) {
            if ( shouldCancel() ) {
                if ( !goodCancel )
                    throw new InterruptedException();

                break; // We need to cancel RecursiveSolvers we have made.
            }

            if ( results.take() ) {
                res = true;
                break;
            }
        }

        for ( RecursiveSolver r : smaller_tasks )
            r.setGoodCancel();

        for ( RecursiveSolver r : smaller_tasks )
            r.cancel(true);

        return res;
    }

    private boolean solveAND( CircuitNode[] args, RecursiveSolver[] smaller_tasks ) throws InterruptedException {
        BlockingQueue<Boolean> results = new LinkedBlockingQueue<>();

        for ( int i = 0; i < smaller_tasks.length; i++ ) {
            if ( shouldCancel() ) {
                if ( !goodCancel )
                    throw new InterruptedException();

                return false;
            }

            smaller_tasks[i] = new RecursiveSolver( args[i], results, solver );
        }

        for ( RecursiveSolver r : smaller_tasks ) {
            if ( shouldCancel() ) break; // We need to cancel RecursiveSolvers we have made.

            r.fork();
        }

        boolean res = true;
        for ( int i = 0; i < smaller_tasks.length; i++ ) {
            if ( shouldCancel() ) {
                if ( !goodCancel )
                    throw new InterruptedException();

                break; // We need to cancel RecursiveSolvers we have made.
            }

            if ( !results.take() ) {
                res = false;
                break;
            }
        }

        for ( RecursiveSolver r : smaller_tasks )
            r.setGoodCancel();

        for ( RecursiveSolver r : smaller_tasks )
            r.cancel(true);

        return res;
    }

    private boolean solveIF( CircuitNode[] args ) throws InterruptedException {
        boolean b = recursiveSolve( args[0], null );
        return b ? recursiveSolve( args[1], null ) : recursiveSolve( args[2], null );
    }
}
