<!DOCTYPE html>
<html lang="en">
<head>
  <!-- Required meta tags -->
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">

  <!-- Project Title -->
  <title>Valkyria</title>
</head>
<body>
  <!-- Project Logo and Name -->
  <div align="center">
    <a href="https://github.com/valkyria-dev/valkyria">
      <img src="images/logo.svg" alt="Logo" width="80" height="80">
    </a>

    <h3 align="center">Valkyria</h3>

    <!-- Project Description -->
    <p align="center">
      An XML-configurable HTTP server in C++ with a focus on simplicity and customization.
      <br />
      <a href="https://github.com/valkyria-dev/valkyria"><strong>Explore the docs »</strong></a>
      <br />
      <br />
      <a href="https://github.com/valkyria-dev/valkyria">View Demo</a>
      ·
      <a href="https://github.com/valkyria-dev/valkyria/issues">Report Bug</a>
      ·
      <a href="https://github.com/valkyria-dev/valkyria/issues">Request Feature</a>
    </p>
  </div>

  <!-- Table of Contents -->
  <details>
    <summary>Table of Contents</summary>
    <ol>
      <li>
        <a href="#about-the-project">About The Project</a>
        <ul>
          <li><a href="#built-with">Built With</a></li>
        </ul>
      </li>
      <li>
        <a href="#getting-started">Getting Started</a>
        <ul>
          <li><a href="#prerequisites">Prerequisites</a></li>
          <li><a href="#installation">Installation</a></li>
        </ul>
      </li>
      <li><a href="#usage">Usage</a></li>
      <li><a href="#roadmap">Roadmap</a></li>
      <li><a href="#contributing">Contributing</a></li>
      <li><a href="#license">License</a></li>
      <li><a href="#contact">Contact</a></li>
      <li><a href="#acknowledgments">Acknowledgments</a></li>
    </ol>
  </details>

  <!-- About The Project Section -->
  <section id="about-the-project">
    <h2>About The Project</h2>
    <p>
      Valkyria is a simple HTTP server written in C++ that allows you to configure routes and settings using XML files. The server is designed with simplicity and ease of customization in mind. You can define routes, ports, root paths, and more without touching the C++ code.
    </p>
    <p>
      Of course, this project is a starting point, and you are encouraged to modify it according to your needs.
    </p>

    <!-- Built With Section -->
    <h3 id="built-with">Built With</h3>
    <ul>
      <li>C++</li>
      <li>XML Configuration</li>
    </ul>
  </section>

  <!-- Getting Started Section -->
  <section id="getting-started">
    <h2>Getting Started</h2>

    <!-- Prerequisites Section -->
    <h3 id="prerequisites">Prerequisites</h3>
    <ul>
      <li>C++ Compiler</li>
      <li>Git</li>
    </ul>

    <!-- Installation Section -->
    <h3 id="installation">Installation</h3>
    <ol>
      <li>
        Clone the repo
        <pre><code>git clone https://github.com/valkyria-dev/valkyria.git
cd valkyria</code></pre>
      </li>
      <li>
        Build the server
        <pre><code>make</code></pre>
      </li>
      <li>
        Run the server
        <pre><code>./server</code></pre>
      </li>
    </ol>
  </section>

  <!-- Usage Section -->
  <section id="usage">
    <h2>Usage</h2>

    <p>
      Valkyria is configured using XML files. Below is an example XML configuration (<code>config.xml</code>):
    </p>

    <!-- Example XML Configuration -->
    <pre><code>&lt;config&gt;
    &lt;port&gt;3025&lt;/port&gt;
    &lt;rootPath&gt;/var/www/html&lt;/rootPath&gt;
    &lt;interactiveMode&gt;true&lt;/interactiveMode&gt;
    &lt;routes&gt;
        &lt;route path="/" content="&lt;h1&gt;Welcome to the homepage&lt;/h1&gt;" /&gt;
        &lt;route path="/about" content="&lt;h1&gt;About Us&lt;/h1&gt;" /&gt;
    &lt;/routes&gt;
&lt;/config&gt;</code></pre>

    <p>
      This example configures the server to listen on port 3025, set the root path to <code>/var/www/html</code>, enable interactive mode, and define two routes ("/" and "/about").
    </p>

    <p>
      To run the server with this configuration:
    </p>

    <pre><code>./server config.xml</code></pre>
  </section>

  <!-- Roadmap Section -->
  <section id="roadmap">
    <h2>Roadmap</h2>

    <ul>
      <li>XML-based configuration</li>
      <li>Basic routing functionality</li>
      <li>SSL/TLS support</li>
      <li>Improved error handling</li>
      <li>Support for additional HTTP methods</li>
      <li>Multi-threading support</li>
    </ul>

    <p>
      See the <a href="https://github.com/valkyria-dev/valkyria/issues">open issues</a> for a full list of proposed features and enhancements.
    </p>
  </section>

  <!-- Contributing Section -->
  <section id="contributing">
    <h2>Contributing</h2>

    <p>
      Contributions are welcome! If you have suggestions, bug reports, or want to contribute code, please follow these steps:
    </p>

    <ol>
      <li>Fork the Project</li>
      <li>Create your Feature Branch (<code>git checkout -b feature/AmazingFeature</code>)</li>
      <li>Commit your Changes (<code>git commit -m 'Add some AmazingFeature'</code>)</li>
      <li>Push to the Branch (<code>git push origin feature/AmazingFeature</code>)</li>
      <li>Open a Pull Request</li>
    </ol>
  </section>

  <!-- License Section -->
  <section id="license">
    <h2>License</h2>
    <p>
      Distributed under the MIT License. See <code>LICENSE</code> for more information.
    </p>
  </section>

  <!-- Contact Section -->
  <section id="contact">
    <h2>Contact</h2>
    <p>
      Valkyria Team - valkyria@example.com
    </p>
    <p>
      Project Link: <a href="https://github.com/valkyria-dev/valkyria">https://github.com/valkyria-dev/valkyria</a>
    </p>
  </section>

  <!-- Acknowledgments Section -->
  <section id="acknowledgments">
    <h2>Acknowledgments</h2>

    <p>
      Use this space to list resources you find helpful and would like to give credit to. I've included a few of my favorites to kick things off!
    </p>

    <ul>
      <li><a href="https://choosealicense.com">Choose an Open Source License</a></li>
      <li><a href="https://www.webpagefx.com/tools/emoji-cheat-sheet">GitHub Emoji Cheat Sheet</a></li>
      <li><a href="https://flexbox.malven.co/">Malven's Flexbox Cheatsheet</a></li>
      <li><a href="https://grid.malven.co/">Malven's Grid Cheatsheet</a></li>
      <li><a href="https://shields.io">Img Shields</a></li>
      <li><a href="https://pages.github.com">GitHub Pages</a></li>
      <li><a href="https://fontawesome.com">Font Awesome</a></li>
      <li><a href="https://react-icons.github.io/react-icons/search">React Icons</a></li>
    </ul>
  </section>
</body>
</html>
