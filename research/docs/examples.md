# Bernoulli Types: Practical Examples

This document provides concrete examples of Bernoulli types in practice, demonstrating how the theoretical framework applies to real-world problems.

## Table of Contents

1. [Bloom Filter: The Canonical Example](#bloom-filter-the-canonical-example)
2. [Miller-Rabin: Randomized Primality Testing](#miller-rabin-randomized-primality-testing)
3. [Count-Min Sketch: Frequency Estimation](#count-min-sketch-frequency-estimation)
4. [MinHash: Set Similarity](#minhash-set-similarity)
5. [Function Equality Testing](#function-equality-testing)
6. [Approximate Nearest Neighbors](#approximate-nearest-neighbors)
7. [Network Protocols](#network-protocols)
8. [Machine Learning Applications](#machine-learning-applications)

## Bloom Filter: The Canonical Example

### Problem
Store a set S efficiently while allowing membership queries.

### Latent/Observed Model
- **Latent**: Perfect set S with exact membership function
- **Observed**: Bloom filter ~S with probabilistic membership

### Implementation
```python
class BloomFilter:
    def __init__(self, size, num_hashes, seeds):
        self.bits = [False] * size
        self.hashes = [lambda x, s=s: hash((x, s)) % size for s in seeds]
    
    def add(self, item):
        for h in self.hashes:
            self.bits[h(item)] = True
    
    def contains(self, item):  # This is ~contains, not exact contains
        return all(self.bits[h(item)] for h in self.hashes)
```

### Error Analysis
- False positive rate: α = (1 - e^(-kn/m))^k
- False negative rate: β = 0
- Optimal k = (m/n) ln 2

### Use Cases
- Web crawlers (URL deduplication)
- Database query optimization
- Network routing tables
- Spell checkers

## Miller-Rabin: Randomized Primality Testing

### Problem
Determine if a large number n is prime.

### Latent/Observed Model
- **Latent**: isPrime(n) - true primality (exponential time)
- **Observed**: ~isPrime_k(n) - probabilistic test (polynomial time)

### Implementation
```python
def miller_rabin(n, k):
    """k iterations of Miller-Rabin test"""
    if n < 2: return False
    if n == 2 or n == 3: return True
    if n % 2 == 0: return False
    
    # Write n-1 = 2^r * d
    r, d = 0, n - 1
    while d % 2 == 0:
        r += 1
        d //= 2
    
    # Test k witnesses
    for _ in range(k):
        a = random.randrange(2, n - 1)
        x = pow(a, d, n)
        
        if x == 1 or x == n - 1:
            continue
            
        for _ in range(r - 1):
            x = pow(x, 2, n)
            if x == n - 1:
                break
        else:
            return False  # Composite
    
    return True  # Probably prime
```

### Error Analysis
- P(composite reported as prime) ≤ 4^(-k)
- For cryptographic use: k = 40 gives error < 2^(-80)

### Real-World Impact
- RSA key generation
- Elliptic curve cryptography
- Randomized algorithms in number theory

## Count-Min Sketch: Frequency Estimation

### Problem
Track frequency of items in a stream with limited memory.

### Latent/Observed Model
- **Latent**: Exact frequency function freq: Item → ℕ
- **Observed**: Sketch query ~freq: Item → ℕ with overestimation

### Implementation
```python
class CountMinSketch:
    def __init__(self, width, depth, seeds):
        self.counts = [[0] * width for _ in range(depth)]
        self.hashes = [lambda x, s=s: hash((x, s)) % width for s in seeds]
    
    def update(self, item, count=1):
        for i, h in enumerate(self.hashes):
            self.counts[i][h(item)] += count
    
    def query(self, item):  # Returns ~freq(item) ≥ freq(item)
        return min(self.counts[i][h(item)] 
                   for i, h in enumerate(self.hashes))
```

### Error Analysis
- Always: ~freq(x) ≥ freq(x) (no underestimation)
- P(~freq(x) > freq(x) + ε||f||₁) < δ
- Space: O(1/ε log 1/δ)

### Applications
- Network traffic monitoring
- Natural language processing
- Database query optimization
- Streaming algorithms

## MinHash: Set Similarity

### Problem
Estimate Jaccard similarity between large sets.

### Latent/Observed Model
- **Latent**: J(A,B) = |A∩B|/|A∪B|
- **Observed**: ~J(A,B) from signature comparison

### Implementation
```python
class MinHash:
    def __init__(self, num_hashes, seeds):
        self.hashes = [lambda x, s=s: hash((x, s)) for s in seeds]
        
    def signature(self, items):
        return [min(h(item) for item in items) for h in self.hashes]
    
    def similarity(self, sig1, sig2):
        return sum(a == b for a, b in zip(sig1, sig2)) / len(sig1)
```

### Error Analysis
- E[~J] = J (unbiased estimator)
- Var[~J] = J(1-J)/k
- Concentration: P(|~J - J| > ε) < 2exp(-2kε²)

### Use Cases
- Near-duplicate detection
- Recommendation systems
- Clustering large datasets
- Plagiarism detection

## Function Equality Testing

### Problem
Verify two functions are identical without checking all inputs.

### Latent/Observed Model
- **Latent**: f = g (exact equality over entire domain)
- **Observed**: ~(f = g) based on sampled points

### Implementation
```python
def test_function_equality(f, g, domain_sampler, n_samples):
    """Test if f = g by sampling n points"""
    for _ in range(n_samples):
        x = domain_sampler()
        if f(x) != g(x):
            return False  # Definitely different
    return True  # Probably equal
```

### Error Analysis
If f ≠ g on fraction p of domain:
- P(test passes) = (1-p)^n
- To achieve error ε: n ≥ log(ε)/log(1-p)

### Applications
- Compiler optimization verification
- Hardware verification
- Metamorphic testing
- Numerical algorithm validation

## Approximate Nearest Neighbors

### Problem
Find nearest neighbors in high-dimensional space quickly.

### Latent/Observed Model
- **Latent**: Exact nearest neighbor
- **Observed**: Approximate neighbor within (1+ε) factor

### LSH Implementation
```python
class LSH:
    def __init__(self, dim, num_tables, hashes_per_table):
        self.tables = [defaultdict(list) for _ in range(num_tables)]
        self.hash_funcs = self._generate_hashes(dim, num_tables, hashes_per_table)
    
    def _hash(self, point, table_idx):
        return tuple(h(point) for h in self.hash_funcs[table_idx])
    
    def insert(self, point, label):
        for i, table in enumerate(self.tables):
            table[self._hash(point, i)].append((point, label))
    
    def query(self, point, k):
        candidates = set()
        for i, table in enumerate(self.tables):
            candidates.update(table[self._hash(point, i)])
        
        # Return k nearest from candidates
        return sorted(candidates, key=lambda x: distance(x[0], point))[:k]
```

### Error Analysis
- Returns (1+ε)-approximate nearest neighbor
- Success probability ≥ 1-δ
- Query time: O(n^ρ) where ρ < 1

## Network Protocols

### TCP Sequence Numbers as Observations

**Problem**: Ensure reliable delivery over unreliable network.

**Latent/Observed**:
- **Latent**: Perfect in-order delivery
- **Observed**: Packets with sequence numbers (may be lost/reordered)

### Eventual Consistency

**Problem**: Maintain consistency across distributed system.

**Latent/Observed**:
- **Latent**: Instantaneous global consistency
- **Observed**: Eventual consistency with bounded staleness

## Machine Learning Applications

### Dropout as Bernoulli Sampling

```python
def dropout_layer(x, rate=0.5, training=True):
    if not training:
        return x
    
    # Each neuron is a Bernoulli observation of the full network
    mask = np.random.binomial(1, 1-rate, x.shape)
    return x * mask / (1-rate)  # Scale to maintain expectation
```

### Differential Privacy

Adding noise to queries creates Bernoulli observations:

```python
def private_count(true_count, epsilon):
    # Laplace mechanism
    noise = np.random.laplace(0, 1/epsilon)
    return true_count + noise  # ~count with privacy guarantee
```

## Key Insights from Examples

1. **Ubiquity**: Bernoulli types appear everywhere in computer science
2. **Trade-offs**: Space/time/accuracy can be exchanged systematically
3. **Composition**: Errors compound in predictable ways
4. **Guarantees**: Probabilistic bounds often sufficient for practice
5. **Optimality**: Many constructions approach information-theoretic limits

## Design Patterns

### Pattern 1: Multiple Independent Observations
```python
def improve_accuracy(observations):
    """Majority vote over multiple observations"""
    return max(set(observations), key=observations.count)
```

### Pattern 2: Progressive Refinement
```python
def progressive_estimate(oracle, confidence_target):
    """Keep sampling until confidence threshold met"""
    observations = []
    while confidence(observations) < confidence_target:
        observations.append(oracle())
    return aggregate(observations)
```

### Pattern 3: Adaptive Error Rates
```python
def adaptive_structure(target_fpr):
    """Adjust parameters based on observed data"""
    if high_load():
        return FastApproximate(fpr=0.1)
    else:
        return PreciseApproximate(fpr=target_fpr)
```

These examples demonstrate that the Bernoulli framework is not merely theoretical—it provides a lens for understanding and improving real systems we use every day.