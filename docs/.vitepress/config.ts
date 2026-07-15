import { defineConfig } from 'vitepress'

export default defineConfig({
  title: 'FastQueue',
  description: 'Production-Ready Job System & Thread Pool Library (C23)',
  lang: 'en-US',
  lastUpdated: true,
  cleanUrls: true,

  head: [
    ['meta', { name: 'theme-color', content: '#3c8f3c' }],
    ['meta', { name: 'og:type', content: 'website' }],
    ['meta', { name: 'og:title', content: 'FastQueue' }],
    ['meta', { name: 'og:description', content: 'High-performance C23 thread pool and job system library' }],
    ['meta', { name: 'og:url', content: 'https://muhammad-fiaz.github.io/FastQueue/' }],
    ['meta', { name: 'og:site_name', content: 'FastQueue' }],
    ['meta', { name: 'og:image', content: 'https://muhammad-fiaz.github.io/FastQueue/og-image.png' }],
    ['meta', { name: 'twitter:card', content: 'summary_large_image' }],
    ['meta', { name: 'twitter:title', content: 'FastQueue' }],
    ['meta', { name: 'twitter:description', content: 'High-performance C23 thread pool and job system library' }],
    ['meta', { name: 'twitter:image', content: 'https://muhammad-fiaz.github.io/FastQueue/og-image.png' }],
    ['link', { rel: 'icon', type: 'image/svg+xml', href: '/logo.svg' }],
    // Google Tag Manager
    ['script', { src: 'https://www.googletagmanager.com/gtm.js?id=GTM-P4M9T8ZR' }],
    // Google Analytics
    ['script', { async: true, src: 'https://www.googletagmanager.com/gtag/js?id=G-6BVYCRK57P' }],
    ['script', {}, `
      window.dataLayer = window.dataLayer || [];
      function gtag(){dataLayer.push(arguments);}
      gtag('js', new Date());
      gtag('config', 'G-6BVYCRK57P');
    `],
    // Google AdSense
    ['script', { async: true, src: 'https://pagead2.googlesyndication.com/pagead/js/adsbygoogle.js?client=ca-pub-2040560600290490', crossorigin: 'anonymous' }],
  ],

  themeConfig: {
    logo: '/logo.svg',

    nav: [
      { text: 'Guide', link: '/guide/getting-started' },
      { text: 'API', link: '/api/overview' },
      { text: 'GitHub', link: 'https://github.com/muhammad-fiaz/FastQueue' },
    ],

    sidebar: {
      '/guide/': [
        {
          text: 'Guide',
          items: [
            { text: 'Getting Started', link: '/guide/getting-started' },
            { text: 'Installation', link: '/guide/installation' },
            { text: 'Quick Start', link: '/guide/quick-start' },
            { text: 'Thread Pool', link: '/guide/thread-pool' },
            { text: 'Scheduler', link: '/guide/scheduler' },
            { text: 'Futures', link: '/guide/futures' },
            { text: 'Memory', link: '/guide/memory' },
            { text: 'Performance', link: '/guide/performance' },
            { text: 'FAQ', link: '/guide/faq' },
            { text: 'Troubleshooting', link: '/guide/troubleshooting' },
          ],
        },
      ],
      '/api/': [
        {
          text: 'API Reference',
          items: [
            { text: 'Overview', link: '/api/overview' },
            { text: 'Thread Pool', link: '/api/thread_pool' },
            { text: 'Scheduler', link: '/api/scheduler' },
            { text: 'Queue', link: '/api/queue' },
            { text: 'Future', link: '/api/future' },
            { text: 'Task', link: '/api/task' },
            { text: 'Memory', link: '/api/memory' },
            { text: 'Errors', link: '/api/errors' },
            { text: 'Config', link: '/api/config' },
            { text: 'Version', link: '/api/version' },
          ],
        },
      ],
    },

    socialLinks: [
      { icon: 'github', link: 'https://github.com/muhammad-fiaz/FastQueue' },
    ],

    footer: {
      message: 'Released under the MIT License.',
      copyright: 'Copyright 2026 Muhammad Fiaz',
    },

    search: {
      provider: 'local',
    },

    editLink: {
      pattern: 'https://github.com/muhammad-fiaz/FastQueue/edit/main/docs/:path',
      text: 'Edit this page',
    },

    outline: [2, 3],

    carbonAds: {
      enabled: false,
    },
  },

  sitemap: {
    hostname: 'https://muhammad-fiaz.github.io/FastQueue',
  },
})
