
# Neural netorks models

## Getting started


## Build

Configure compiler environment
```bash
$ autoreconf -i
```

Go to build directory, configure and compile
```bash
$ mkdir -p ./build && cd ./build
$ ../configure 
$ make
```

Run tests
```bash
$ make check
```

Run program
```bash
$ ./src/neural_network 
```

Oneliner for rebuild
```bash
$ cd ./build
$ cd ..; autoreconf -i; cd ./build; ../configure && make && make check
```

WIP

In neural networks, neurons typically process information through an **activation function** (the main function) and sometimes utilize additional **secondary functions** for specialized processing tasks. Here’s a breakdown of the most common activation functions and some secondary functions those neurons might employ.

### Common Activation Functions (Main Functions)

1. **Sigmoid / Logistic Function**:
   - **Formula**: \(\sigma(x) = \frac{1}{1 + e^{-x}}\)
   - **Usage**: Often used in binary classification problems to model probabilities. Suitable for the output layer.
   - **Characteristics**: Outputs a value between 0 and 1. Can suffer from vanishing gradients in deep networks.

2. **Hyperbolic Tangent (Tanh)**:
   - **Formula**: \(\tanh(x) = \frac{e^x - e^{-x}}{e^x + e^{-x}}\)
   - **Usage**: Common in hidden layers. Output values are between -1 and 1, which usually results in faster convergence than sigmoid.
   - **Characteristics**: Zero-centered output, reducing some biases during optimization.

3. **Rectified Linear Unit (ReLU)**:
   - **Formula**: \(f(x) = \max(0, x)\)
   - **Usage**: Extensively used in modern deep networks, especially in convolutional and feedforward networks.
   - **Characteristics**: Simplicity and effectiveness. Does not suffer from vanishing gradient for positive parts. Can suffer from "dying ReLU" problem where neurons become inactive.

4. **Leaky ReLU and Parametric ReLU (PReLU)**:
   - **Formula**: \(f(x) = \max(\alpha x, x)\) for Leaky ReLU; \(\alpha\) is small, like 0.01.
   - **Usage**: Alternative to ReLU to mitigate the dying ReLU problem by allowing a small, non-zero gradient for negative inputs.
   - **Characteristics**: Helps to keep some learning alive during training, even for negative inputs.

5. **Softmax Function**:
   - **Formula**: \(\sigma(\mathbf{x})_i = \frac{e^{x_i}}{\sum_{j} e^{x_j}}\)
   - **Usage**: Commonly used in the output layer of classifiers for multi-class classification.
   - **Characteristics**: Converts logits to probability distributions over different classes.

### Secondary Functions (Utility/Supporting Functions)

1. **Dropout Function**:
   - **Purpose**: Temporarily drops neurons during training to prevent overfitting by ensuring that a neuron doesn't rely on any specific other neuron in its activity.
   - **Usage**: Applied during training only, not during inference. Used in fully connected layers of a network.

2. **Batch Normalization**:
   - **Purpose**: Normalize activations of a previous layer at each batch, improving convergence rates by maintaining mean and variance of activations.
   - **Usage**: Often used to combat internal covariate shift, increasing the stability and performance of networks.

3. **Pooling Functions**:
   - **Types**: Max Pooling, Average Pooling
   - **Purpose**: Down-samples the input dimensions by reducing spatial size, which helps to reduce the number of parameters and computation in the network.
   - **Usage**: Used in convolutional neural networks (CNNs).

4. **Regularization Techniques**:
   - **Types**: L1 Regularization, L2 Regularization
   - **Purpose**: Add penalty terms to prevent overfitting by discouraging complex models through cost functions.
   - **Usage**: Often used alongside loss functions.

5. **Rectification**:
   - **Type**: Clipping gradients
   - **Purpose**: Prevents gradients from exploding by clipping them within a specific range.
   - **Usage**: Commonly used in recurrent neural networks (RNNs).

These functions represent a comprehensive set of processing and operational tasks neurons in neural networks engage in during model training and evaluation, contributing significantly to their learning and generalization performance.
